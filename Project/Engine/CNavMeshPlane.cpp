#include "pch.h"
#include "CNavMeshPlane.h"
#include "CTransform.h"
#include "CKeyMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CNavMeshMgr.h"
#include "CMeshRender.h"

UINT CNavMeshPlane::m_iObstacleCount = 0;

CNavMeshPlane::CNavMeshPlane():
	CComponent(COMPONENT_TYPE::NAVMESHPLANE),
	m_bActiveRay(false),
	m_bActiveCreate(false),
	m_bObstaclePlane(false),
	m_vecTargetPlane{}
{
}

CNavMeshPlane::~CNavMeshPlane()
{

}


void CNavMeshPlane::finaltick()
{
	DrawCube();

	//장애물 오브젝트는 레이케스팅이 필요 없음
	if (m_bObstaclePlane)
		return;

	if (KEY_TAP(KEY::NUM_0))
		m_bActiveCreate = true;
	else if (KEY_TAP(KEY::NUM_1))
		m_bActiveCreate = false;
	
	if (!m_bActiveCreate)
		return;
	
	if (KEY_PRESSED(KEY::LBTN))
	{
		if (RayCasting())
			CreateObstacle();
	}
	else if (KEY_RELEASE(KEY::LBTN) && m_bActiveRay)
	{
		ReBulid();
	}	
}

void CNavMeshPlane::CreateObstacle()
{
	//한 번만 생성하고 그 후 크기 조절
	if (m_vecTargetPlane.empty())
	{
		//navmesh가 아닌 일반 오브젝트로 생성
		for (int i = 0; i < 4; ++i)
			CreateTemObstacle();
	}

	//위치 잡기
	m_vCreateScale = m_vCreateScale + (DT * Vec3(40.f,40.f,40.f));
	Vec3 vHalfScale = m_vCreateScale / 2.f;

	float fHeight = m_vCreateScale.y;
	//오브젝트를 설치하기 위해서 높이값을 크기의 /2 만큼 올린다
	float y = m_vRayPoint.y + fHeight /2.f;
	Vec3 vPos = Vec3(m_vRayPoint.x, y, m_vRayPoint.z);
	
	Vec3 vDir[4] = { Vec3(0.f,0.f,1.f), Vec3(1.f, 0.f, 0.f),
		Vec3(0.f,0.f, -1.f) ,Vec3(-1.f, 0.f , 0.f) };

	Vec3 vScaleDir[2] = { Vec3(1.f,0.f,0.f), Vec3(0.f,0.f,1.f) };

	//2번째가 안됨 (확인 세로축)
	for (int i = 0; i < 4; ++i)
	{
		//정행진 방향으로 메쉬 위치 지정
		Vec3 vPlanePos = vDir[i] * vHalfScale + vPos;

		//크기 적용
		Vec3 vScale = vScaleDir[i%2] * m_vCreateScale;
		vScale.y += fHeight; //높이값 적용

		
		m_vecTargetPlane[i]->Transform()->SetRelativePos(vPlanePos);
		m_vecTargetPlane[i]->Transform()->SetRelativeScale(vScale);
	}
	
	m_bActiveRay = true;
}

bool CNavMeshPlane::RayCasting()
{
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return false;

	const tRay& ray = pMainCam->GetRay();

	//직선의 방정식
	//R(t) = pOrigin + t*vDir
	//R(t) = p0 - t * vDir    R(t) : 직석상의 모든 점
	// pOrigin : 직선의 임의 점, t : p0로부터 떨어진 거리  vDir : 직선의 방향  


	//평면의 방정식
	//N dot (p0 - p1) = 0
	// N : 평면의 수직인 벡터, p0 : 평면의 임의의 한 점 , p1 : 평면상의 임의한 한 점 p0 - p1 : 평면위에 선분(벡터)

	// S : 직선과 평면의 교점 (여기서 S는 평면과 직선의 교점이기 때문에 평면의 방정식 p1으로 가정, 
	// 직선의 방정식 R(t)도 가능 때문에 p1 = R(t)가 되는 지점이 S가 됨

	//N dot (p0 - R(t)) = 0
	//N dot p0 - N dot R(t) = 0
	//N dot p0 - N dot (pOrigin + t*vDir) = 0
	//N dot p0 - N dot pOrigin - t *(N dot vDir) = 0
	//t*(N dot vDir) = N dot p0 - N dot pOrigin
	// t = N dot p0 - N dot pOrigin / N dot p0


	Vec3 vEdge[3] = {};
	vEdge [0] = m_vecWorldVertices[0] - m_vecWorldVertices[1];
	vEdge [1] = m_vecWorldVertices[2] - m_vecWorldVertices[1];
	vEdge [2] = m_vecWorldVertices[2] - m_vecWorldVertices[0];
	//평면의 노말벡터
	Vec3 vNormal = vEdge->Cross(vEdge[1]);
	//N dot pOrigin / N dot p0
	//float f = vNormal.Dot(m_vecWorldVertices[0]) - vNormal.Dot(ray.vStart);

	//직선과 평면이 평행 , 평면에 포함 이 때 교점은 존재하지 않는다.
	float f = vNormal.Dot(ray.vDir);
	if (f == 0.f)
		return false;

	Vec3 vLen = ray.vStart - m_vecWorldVertices[0];
	float fResult = -vNormal.Dot(vLen);
	float fT = fResult / f;

	Vec3 vPoint = ray.vStart + fT * ray.vDir;

	//교차점이 맞는지 확인
	// 교차 지점이 사각형 내부에 있는지 확인

	// 0 ----3
	// |     |
	// 1-----2
	
	// 외적을 통한 삼각형의 총 넓이 합도 가능함
	
	// 배리 센트릭 좌표 계산
	double uu, uv, vv, wu, wv, inverseD;

	//삼각형 세 점의 가중치
	uu = vEdge[0].Dot(vEdge[0]);
	uv = vEdge[0].Dot(vEdge[1]);
	vv = vEdge[1].Dot(vEdge[1]);

	//삼각형 내 교점 가중치
	Vec3 w = vPoint - m_vecWorldVertices[1];
	wu = w.Dot(vEdge[0]);
	wv = w.Dot(vEdge[1]);

	//베리 센트릭 공식 대입 
	//D는 삼각형의 배리 센트릭 좌표를 구할 때 사용되는 값
	inverseD = uv * uv - uu * vv;
	inverseD = 1.0f / inverseD;

	//가중치  범위가 0~1 일때만 참으로 반환

	//u =        D
	//    (uv*wv - vv*wu)

	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.0f || u > 1.0f) 
		return false;

	//v =         D
	//     (uv*wu - uu*wv)

	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.0f || (u + v) > 1.0f)
		return false;

	m_vRayPoint = vPoint;

	m_bActiveRay = true;

	return true;
}

float CNavMeshPlane::GetArea(const Vec3& _vP1, const Vec3& _vP2)
{
	float fArea =
		(_vP1.x*_vP2.y + _vP1.y*_vP2.z + _vP1.z*_vP2.x) -
		(_vP1.y*_vP2.x + _vP1.z*_vP2.y + _vP1.x*_vP2.z);

	return fArea / 2.f;

}

void CNavMeshPlane::DrawCube()
{
	Vec4 vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	const Matrix& m_tMat = GetOwner()->Transform()->GetWorldMat();

	DrawDebugCube(m_tMat, vColor, 0.0f, false);
}

void CNavMeshPlane::CreateTemObstacle()
{
	CGameObject* pTargetPlane = new CGameObject();

	pTargetPlane->SetName(L"navMeshObstacle" + std::to_wstring(m_iObstacleCount));
	pTargetPlane->AddComponent(new CTransform);
	pTargetPlane->Transform()->SetRelativeScale(Vec3::One);

	pTargetPlane->AddComponent(new CMeshRender);
	pTargetPlane->AddComponent(new CNavMeshPlane());
	SpawnGameObject(pTargetPlane, Vec3::One, (int)LAYER_TYPE::NavMeshPlane);

	m_vecTargetPlane.push_back(pTargetPlane);

	++m_iObstacleCount;
}

void CNavMeshPlane::ReBulid()
{
	//새로 만들어진 오브젝트를 navmeshmgr에 등록
	for (int i = 0; i < m_vecTargetPlane.size(); ++i)
	{
		//장애물 오브젝트로 등록
		m_vecTargetPlane[i]->NavMeshPlane()->m_bObstaclePlane = true;
		CNavMeshMgr::GetInst()->AddPlaneVertex(m_vecTargetPlane[i]->NavMeshPlane());
	}

	//navmesh 빌드 다시 진행 후 초기화
	CNavMeshMgr::GetInst()->ReBuildField();

	m_bActiveRay = false;
	m_bActiveCreate = false;
	m_vecTargetPlane.clear();
}

void CNavMeshPlane::SaveToLevelFile(FILE* _File)
{
}

void CNavMeshPlane::LoadFromLevelFile(FILE* _File)
{
}


