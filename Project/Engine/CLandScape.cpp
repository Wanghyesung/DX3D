#include "pch.h"
#include "CLandScape.h"

#include "CRenderMgr.h"
#include "CResMgr.h"
#include "CKeyMgr.h"

#include "CStructuredBuffer.h"

#include "CTransform.h"
#include "CCamera.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iFaceX(1)
	, m_iFaceZ(1)
	, m_vBrushScale(Vec2(0.2f, 0.2f))
	, m_iWeightIdx(2)
{
	init();

	//만약 저장된 높이, 가중치맵이 있다면 로드
	LoadWeightMap();
	LoadHeightMap();
}

CLandScape::~CLandScape()
{
	if (nullptr != m_pCrossBuffer)
		delete m_pCrossBuffer;

	if (nullptr != m_pWeightMapBuffer)
		delete m_pWeightMapBuffer;
}


void CLandScape::finaltick()
{
	if (KEY_TAP(KEY::NUM_0))
		m_eMod = LANDSCAPE_MOD::NONE;
	else if (KEY_TAP(KEY::_1))
		m_eMod = LANDSCAPE_MOD::HEIGHT_MAP;
	else if (KEY_TAP(KEY::_2))
		m_eMod = LANDSCAPE_MOD::SPLAT;
	else if (KEY_TAP(KEY::_3))
	{
		m_iWeightIdx++;
		if (3 <= m_iWeightIdx)
			m_iWeightIdx = 0;
	}



	//m_eMod = LANDSCAPE_MOD::HEIGHT_MAP;

	if (LANDSCAPE_MOD::NONE == m_eMod)
	{
		return;
	}

	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();

		if (LANDSCAPE_MOD::HEIGHT_MAP == m_eMod)
		{
			// 교점 위치정보를 토대로 높이를 수정 함
			m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 픽킹 정보를 HeightMapShader 에 세팅

			m_pCSHeightMap->SetBrushTex(m_pBrushTex);		// 사용할 브러쉬 텍스쳐 세팅
			m_pCSHeightMap->SetBrushIndex(0);				// 브러쉬 인덱스 설정
			m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // 브러쉬 크기
			m_pCSHeightMap->SetHeightMap(m_HeightMap);
			m_pCSHeightMap->Execute();

		}
		else if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			// 교점 위치정보를 토대로 가중치를 수정함	
			m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // 레이 캐스트 위치
			m_pCSWeightMap->SetBrushArrTex(m_pBrushTex);
			m_pCSWeightMap->SetBrushIndex(0);
			m_pCSWeightMap->SetBrushScale(m_vBrushScale); // 브러쉬 크기
			m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // 가중치맵, 가로 세로 개수			
			m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
			m_pCSWeightMap->Execute();
		}
	}


	if (KEY_TAP(KEY::ENTER))
	{
		SaveWeightMap();
		SaveheightMap();
	}

}

void CLandScape::render()
{
	if (GetMesh() == nullptr || GetMaterial(0) == nullptr)
		return;

	Transform()->UpdateData();

	//GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	GetMaterial(0)->SetScalarParam(INT_0, &m_iFaceX);
	GetMaterial(0)->SetScalarParam(INT_1, &m_iFaceZ);
	GetMaterial(0)->SetTexParam(TEX_2, m_HeightMap);

	Vec2 vResolution = Vec2(m_HeightMap->Width(), m_HeightMap->Height());
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, &vResolution);
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_2, m_HeightMap);
	
	// 가중치 버퍼 전달
	m_pWeightMapBuffer->UpdateData(17, PIPELINE_STAGE::PS_PIXEL);


	// 가중치 버퍼 해상도 전달
	Vec2 vWeightMapResolution = Vec2((float)m_iWeightWidth, (float)m_iWeightHeight);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);

	// 타일 배열 개수 전달
	//3
	float m_fTileCount = float(m_pTileArrTex->GetArraySize() / 2); // 색상, 노말 합쳐져있어서 나누기 2 해줌
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_fTileCount);

	// 타일 텍스쳐 전달
	//배열 형태의 텍스쳐
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

	// 재질 바인딩(업데이트)
	GetMaterial(0)->UpdateData();

	// Camera World Pos 전달
	//Vec3 vCamWorldPos = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldPos();
	//GetMaterial(0)->SetScalarParam(VEC4_0, &vCamWorldPos);

	// 렌더
	GetMesh()->render(0);

	// ==========
	// 리소스 정리
	// ==========
	m_pWeightMapBuffer->Clear();
}

void CLandScape::render(UINT _iSubset)
{
	render();
}



void CLandScape::SetFace(UINT _iFaceX, UINT _iFaceZ)
{
	m_iFaceX = _iFaceX;
	m_iFaceZ = _iFaceZ;

	CreateMesh();
}


void CLandScape::Raycasting()
{
	// 시점 카메라를 가져옴
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// 월드 기준 광선을 지형의 로컬로 보냄
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_pCrossBuffer->SetData(&out);

	m_pCSRaycast->SetHeightMap(m_HeightMap);
	m_pCSRaycast->SetFaceCount(m_iFaceX, m_iFaceZ);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);
	
	m_pCSRaycast->Execute();

	//m_pCrossBuffer->GetData(&out);
	//int a = 0;
}

bool CLandScape::LoadWeightMap()
{
	//파일 경로
	wstring strFileName = L"texture\\tile\\weight.buf";
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + strFileName;

	FILE* pFile = nullptr;
	errno_t err =_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	if (err)
		return false;
	//버퍼 가로 세로
	fread(&m_iWeightWidth, sizeof(int), 1, pFile);
	fread(&m_iWeightHeight, sizeof(int), 1, pFile);

	//배열 데이터 사이즈 저장
	UINT iByteSize = 0;
	fread(&iByteSize, sizeof(int), 1, pFile);

	//배열 전체의 가중치 저장
	vector<tWeight_4> vecWeight;
	vecWeight.resize(iByteSize);
	for (UINT i = 0; i < iByteSize; ++i)
		fread(&vecWeight[i], sizeof(tIndexInfo), 1, pFile);

	m_pWeightMapBuffer->SetData(vecWeight.data());

	fclose(pFile);

	return S_OK;
}

bool CLandScape::SaveWeightMap()
{
	vector<tWeight_4> vecWeight = {};
	vecWeight.resize(m_iWeightWidth * m_iWeightHeight);
	m_pWeightMapBuffer->GetData(vecWeight.data());

	// 파일 경로 만들기
	wstring strFileName = L"texture\\tile\\weight.buf";
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + strFileName;

	// 파일 쓰기모드로 열기
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 버퍼 가로 세로 크기 저장 (크기 동일)
	fwrite(&m_iWeightWidth, sizeof(int), 1, pFile);
	fwrite(&m_iWeightHeight, sizeof(int), 1, pFile);
	
	// 배열 데이터 사이즈 저장
	int iByteSize = vecWeight.size();
	fwrite(&iByteSize, sizeof(int), 1, pFile);
	
	//배열 전체의 가중치 저장
	for (UINT i = 0; i < iByteSize; ++i)
	{
		fwrite(&vecWeight[i], sizeof(tIndexInfo), 1, pFile);
	}

	fclose(pFile);

	return S_OK;
}

bool CLandScape::LoadHeightMap()
{
	Ptr<CTexture> pHeightMap =
		CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\heightmap.png");

	if (pHeightMap != nullptr)
		m_HeightMap = pHeightMap;


	return S_OK;
}

bool CLandScape::SaveheightMap()
{
	// 파일 경로 만들기
	wstring strFileName = L"texture\\tile\\heightmap.png";
	
	HRESULT hr = m_HeightMap->Save(strFileName);

	return hr;
}

void CLandScape::SaveToLevelFile(FILE* _File)
{

}

void CLandScape::LoadFromLevelFile(FILE* _File)
{

}
