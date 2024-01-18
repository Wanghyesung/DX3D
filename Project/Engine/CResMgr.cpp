#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

CResMgr::CResMgr()
	: m_Changed(false)
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{
	InitSound();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();
}



void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32�� ä�� ����
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}


void CResMgr::tick()
{
	m_Changed = false;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

vector<Ptr<CMeshData>> CResMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();

	vector<Ptr<CMeshData>> vecMeshData = {};

	int i = 0;
	while (TRUE)
	{
		wstring strNum = std::to_wstring(i);
		wstring strFileNum = strFileName + strNum;
		wstring strName = L"meshdata\\" + strFileNum + L".mdat";

		Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);

		if (nullptr != pMeshData)
			vecMeshData.push_back(pMeshData);
		else
			break;

		++i;
	}

	if (vecMeshData.size() != 0)
		return vecMeshData;

	//fbx�� ���� �޽������� �ε��� ����, ���׸��� ������ ���� meshdata��ü�� ��ȯ
	vecMeshData = CMeshData::LoadFromFBX(_strPath);
	for (i = 0; i < vecMeshData.size(); ++i)
	{
		wstring strNum = std::to_wstring(i);
		wstring strFileNum = strFileName + strNum;
		wstring strName = L"meshdata\\" + strFileNum + L".mdat";

		vecMeshData[i]->SetKey(strName);
		vecMeshData[i]->SetRelativePath(strName);

		m_arrRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, vecMeshData[i].Get()));

		// meshdata �� �������Ϸ� ����
		vecMeshData[i]->Save(strName);
	}

	//pMeshData->SetKey(strName);
	//pMeshData->SetRelativePath(strName);

	return vecMeshData;
}


void CResMgr::DeleteRes(RES_TYPE _type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_type].find(_strKey);

	assert(!(iter == m_arrRes[(UINT)_type].end()));

	m_arrRes[(UINT)_type].erase(iter);

	m_Changed = true;
}