#include "pch.h"
#include "CNavMesh.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CLandScape.h"
#include "CCollider3D.h"
//내가 온 거리 값 + 목적지까지의 거리 
//이 값이 작은 노드가 가장 먼저 나갈 노드
struct cmp {//우선순위 큐 비교 함수
	bool operator()(const tNode u, const tNode v) {
		if (u.iCurCos + u.iGoalCos > v.iCurCos + v.iGoalCos)
			return true;
		else if (u.iCurCos + u.iGoalCos == v.iCurCos + v.iGoalCos)
		{
			if (u.iCurCos > v.iCurCos)
				return true;
			else
				return false;
		}

		else
			return false;
	}
};

CNavMesh::CNavMesh() :
	CComponent(COMPONENT_TYPE::NAVMESH),
	m_fCurTime(0.f),
	m_fTraceTime(0.15f),
	m_vSearchDir(Vec3::Zero),
	m_vOwnerScale(Vec3::Zero),
	m_bActive(false)
{

}

CNavMesh::~CNavMesh()
{

}

void CNavMesh::begin()
{
	init_map();
}

void CNavMesh::finaltick()
{
	if (!m_bActive)
		return;

	m_fCurTime += DT;

	if (m_fCurTime >= m_fTraceTime)
	{
		m_fCurTime = 0.f;
		tracking_player();
	}
}

void CNavMesh::Initialize()
{

}

void CNavMesh::astar(pair<int, int> _tStart, pair<int, int> _tGoal)
{
	priority_queue<tNode, vector<tNode>, cmp> queueWay;	// 우선순위 큐

	tNode tStartNode;									// 시작 노드

	vector<tNode> vecWay;							//내가 간 길

	int DX[8] = { 1, 1,-1, -1, 0 ,1, 0, -1};				// 방향 좌표 ↘↗↙↖↑→↓← = 시계방향으로
	int DZ[8] = { 1,-1, 1, -1, -1 ,0, 1,  0};

	// 시작지점 초기화
	tStartNode.x = _tStart.second; // x축이 second
	tStartNode.z = _tStart.first; //  z축이 first
	tStartNode.iCurCos = 0; //현재 비용은 0

	tStartNode.iGoalCos = (abs(_tGoal.second - tStartNode.x) + abs(_tGoal.first - tStartNode.z)) * 10;
	tStartNode.pParentNode = make_pair(-1, -1);				// 시작 노드의 부모 노드는 -1,-1
	queueWay.push(tStartNode);
	
	while (!queueWay.empty()) //내 
	{
		int x = queueWay.top().x;						// 우선순위 큐에서 top 정보 추출
		int z = queueWay.top().z;
		int cost = queueWay.top().iCurCos;
		vecWay.push_back(queueWay.top());
		m_vecMap[z][x] = 8; 
		queueWay.pop();
		if (x == _tGoal.second && z == _tGoal.first) break;	

		tNode tAddNode; //주변탐색에 쓰일 노드
		for (int i = 0; i < 8; i++)
		{// top 노드에서 상하좌우 8방향으로 탐색
			int nextX = x + DX[i];
			int nextZ = z + DZ[i];
			//지정된 범위를 벗어나지 않게
			if (nextX >= 0 && nextX < m_vecMap[0].size() && nextZ >= 0 && nextZ < m_vecMap.size())
			{
				//막힌길이거나 내가 온 길이면 다시 못오게
				if (m_vecClose[nextZ][nextX] != 1 && m_vecMap[nextZ][nextX] == false)
				{
					tAddNode.x = nextX;
					tAddNode.z = nextZ;
					tAddNode.iCurCos = i >= 4 ? cost + 10 : cost + 14;// 상하좌우면 10, 대각선이면 14(√200)
					tAddNode.iGoalCos = (abs(_tGoal.second - tAddNode.x) + abs(_tGoal.first - tAddNode.z)) * 10;
					tAddNode.pParentNode = make_pair(z, x);	

					m_vecMap[nextZ][nextX] = 9;		// 탐색한 주변 노드 = (아직 가지 않았으니깐 9로 표시)
					queueWay.push(tAddNode);				// 우선순위 큐에 삽입	
				}
			}
		}
	}

	int px = vecWay.back().x;
	int pz = vecWay.back().z;
	while (vecWay.size())
	{			
		if (px == vecWay.back().x && pz == vecWay.back().z)
		{
			if (vecWay.back().pParentNode.second == _tStart.second &&
				vecWay.back().pParentNode.first == _tStart.first)
				break;

			px = vecWay.back().pParentNode.second;
			pz = vecWay.back().pParentNode.first;
		}
		vecWay.pop_back(); //내가 봤던 길
	}

	Vec3 vDir;
	if (vecWay.size() <= 1)
		vDir = Vec3::Zero;
	else
		vDir = Vec3(px, 0.f, pz) - Vec3(vecWay[0].x, 0.f, vecWay[0].z);

	vDir.Normalize();
	m_vSearchDir = vDir;
}

void CNavMesh::init_map()
{
	const vector<CGameObject*>& vecLand =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::LandScape)->GetParentObject();

	if (vecLand.size() == 0)
		return;
	//일단 가장 처음 지형만 탐색
	//지형 가장 
	CGameObject* pScape = vecLand[0];
	Vec3 vScapePos = pScape->Collider3D()->GetWorldPos(); //9000, 9000
	Vec3 vScale = pScape->Collider3D()->GetOffsetScale(); // 18000 18000
	vScale /= 2.f;

	Vec3 vRightTop = vScapePos + vScale;
	Vec3 vLeftDown = vScapePos - vScale;
	m_vLandScapeLen = vRightTop - vLeftDown;
	//Vec3 vCapeLen 
	//충돌체 x, z크기의 2차원 배열
	m_vOwnerScale = GetOwner()->Collider3D()->GetOffsetScale();
	//float fTem = m_vOwnerScale.z;
	//m_vOwnerScale.z = m_vOwnerScale.y;
	//m_vOwnerScale.y = fTem;

	Vec3 vDivideSize = m_vLandScapeLen / m_vOwnerScale; // 150 150사이즈 몬스터가 지나갈 수 있는 범위로 설계 
	//120개
	m_iDivideX =  vDivideSize.x; //전체 지형을 나눌 수
	m_iDivideZ =  vDivideSize.z;

	m_vecClose = vector<vector<int>>(m_iDivideZ, vector<int>(m_iDivideX));

	//--- 지형 못가는 쪽 생성
	init_closemap();
}

void CNavMesh::init_closemap()
{
	const vector<CGameObject*>& vecLandform =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Obstacle)->GetObjects();

	for (int i = 0; i < vecLandform.size(); ++i)
	{
		if (!vecLandform[i]->Collider3D())
			continue;

		Vec3 vformPos = vecLandform[i]->Collider3D()->GetWorldPos();

		//크기
		Vec3 vformScale = vecLandform[i]->Collider3D()->GetOffsetScale();
		vformScale /= 2.f;
		//좌 우 하 상
		float fPos[4] = { vformPos.x - vformScale.x, vformPos.x + vformScale.x,
						 vformPos.z - vformScale.z, vformPos.z + vformScale.z };

		//칸당 갈수있는 길이
		float fOffsetX = m_iDivideX /3.f;
		float fOffsetZ = m_iDivideZ /3.f;

		float fCorrectionZ = vformScale.z / 3.f;
		float fCorrectionX = vformScale.x / 3.f;

		for (float i = fPos[2] + fCorrectionZ; i <= fPos[3] - fCorrectionZ; i += fOffsetZ)//z
		{
			for (float j = fPos[0] + fCorrectionX; j <= fPos[1] - fCorrectionX; j += fOffsetX) //x
			{
				//현재 검사하는 위치
				Vec3 vPos = Vec3{ j, 0.f, i };

				//맵에서의 비율을 잡기
				Vec3 vRatio = {}; 
				vRatio.x = vPos.x / m_vOwnerScale.x;
				vRatio.z = vPos.z / m_vOwnerScale.z;

				//Vec3 vAbsRatio = Vec3(abs(vRatio.x), abs(vRatio.y), abs(vRatio.z));

				//맵에서의 비율을 잡기
				//vRatio.x /= m_iDivideX; //0.5 * 40 = 20
				//vRatio.z /= m_iDivideZ;
				//못가는 길
				m_vecClose[(int)vRatio.z][(int)vRatio.x] = 1;
			}
		}
	}
}

void CNavMesh::tracking_player()
{
	if (m_iDivideZ == 0)
		return;

	m_vecMap = vector<vector<int>>(m_iDivideZ, vector<int>(m_iDivideX));

	const vector<CGameObject*>& vecPlayer =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject();

	CGameObject* pPlayer = vecPlayer[0];
	if (pPlayer->IsDead())
		return;

	
	//플레이어 현재 몬스터 위치 비율로 잡아서 맵에 넣기
	Vec3 vPlayerPos = pPlayer->Collider3D()->GetWorldPos();

	Vec3 vRatio = {};

	vRatio.x = vPlayerPos.x / m_vOwnerScale.x;
	vRatio.z = vPlayerPos.z / m_vOwnerScale.z;

	//m_vecMap[(int)vRatio.x][(int)vRatio.z] = 5;//목적지는 5
	Vec3 vGoalPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);
	// --몬스터

	CGameObject* pMonster = GetOwner();
	Vec3 vMonsterPos = pMonster->Collider3D()->GetWorldPos();

	//vRatio = vMonsterPos / m_iDivideX;
	vRatio.x = vMonsterPos.x / m_vOwnerScale.x;
	vRatio.z = vMonsterPos.z / m_vOwnerScale.z;
	//m_vecMap[(int)vRatio.x][(int)vRatio.z] = 6;//시작 위치는 6	
	Vec3 vStartPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);


	pair<int, int> tStart = make_pair(vStartPos.z, vStartPos.x);
	pair<int, int> tGoal = make_pair(vGoalPos.z, vGoalPos.x);

	//길찾기
	astar(tStart, tGoal);
}

void CNavMesh::SaveToLevelFile(FILE* _File)
{

}

void CNavMesh::LoadFromLevelFile(FILE* _FILE)
{

}

