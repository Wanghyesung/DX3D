#include "pch.h"
#include "CNavMeshMgr.h"

#include "CTimeMgr.h"
#include "CGameObject.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CLandScape.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCollider3D.h"

CNavMeshMgr::CNavMeshMgr():
	m_mapNavInfo{},
	m_vecMap{},
	m_vecClose{}
{

}

CNavMeshMgr::~CNavMeshMgr()
{

}

//내가 온 거리 값 + 목적지까지의 거리 
//이 값이 작은 노드가 가장 먼저 나갈 노드
struct cmp {//우선순위 큐 비교 함수
	bool operator()(const tNode u, const tNode v) {
		if (u.iCurCos + u.iGoalCos > v.iCurCos + v.iGoalCos)
			return true;
		else if (u.iCurCos + u.iGoalCos == v.iCurCos + v.iGoalCos)
		{
			if (u.iCurCos < v.iCurCos)
				return true;
			else
				return false;
		}

		else
			return false;
	}
};

void CNavMeshMgr::AddMonster(CGameObject* pGameObj , float _fSearchTime)
{
	int iID = pGameObj->GetID();

	map<int, tNavInfo>::iterator iter =
		m_mapNavInfo.find(iID);

	if (iter != m_mapNavInfo.end())
		return;

	tNavInfo tNav = {};
	tNav.iID = iID;
	tNav.fCurTime = 0.f;
	tNav.fEndTime = _fSearchTime;
	tNav.vCurPos = Vec3::Zero;
	tNav.vGoalPos = Vec3::Zero;
	tNav.bSearchOn = false;

	m_mapNavInfo.insert(make_pair(iID, tNav));
}

void CNavMeshMgr::DeleteMonster(CGameObject* pGameObj)
{
	int iID = pGameObj->GetID();

	map<int, tNavInfo>::iterator iter =
		m_mapNavInfo.find(iID);

	if (iter != m_mapNavInfo.end())
		return;

	m_mapNavInfo.erase(iID);
}


void CNavMeshMgr::astar(int _ID , pair<int, int> _tStart, pair<int, int> _tGoal)
{
	priority_queue<tNode, vector<tNode>, cmp> queueWay;	// 우선순위 큐

	tNode tStartNode;									// 시작 노드

	vector<tNode> vecWay;							//내가 간 길

	int DX[8] = { 0 ,1, 0, -1, 1, 1,-1, -1 };				// 방향 좌표 ↑→↓←↘↗↙↖ = 시계방향으로
	int DZ[8] = {-1 ,0, 1,  0, 1,-1, 1, -1 };

	// 시작지점 초기화
	tStartNode.x = _tStart.second; // x축이 second
	tStartNode.z = _tStart.first; //  z축이 first
	tStartNode.iCurCos = 0; //현재 비용은 0

	//여기서 10은 1 : 1 : √2를 계산할때 쉽게 나타내기 위해서 10을 곱해서 표현한 것입니다
	//상하좌우 움직임 비용은 10 대각선 비용은 14
	//내 시작 노드에서 목표까지의 최종 값을 계산합니다 (x축으로 얼마나 먼지 z축으로 얼마나 먼지)
	tStartNode.iGoalCos = ((_tGoal.second - tStartNode.x) + abs(_tGoal.first - tStartNode.z)) * 10;
	tStartNode.pParentNode = make_pair(-1, -1);				// 시작 노드의 부모 노드는 -1,-1
	queueWay.push(tStartNode);
	m_vecClose[tStartNode.z][tStartNode.x] = true;				// 내가 간 지점(다시 못 오게)

	while (!queueWay.empty()) //내 
	{
		int x = queueWay.top().x;						// 우선순위 큐에서 top 정보 추출
		int z = queueWay.top().z;
		int cost = queueWay.top().iCurCos;
		vecWay.push_back(queueWay.top());
		m_vecMap[z][x] = 8; //제가 간 길을 기록해두기 위해서 8로 설정(내가 간 길 , 현재 이번 노드)
		queueWay.pop();
		if (x == _tGoal.second && z == _tGoal.first) break;	// 도착 지점이 나오면 끝

		tNode tAddNode; //주변탐색에 쓰일 노드
		for (int i = 0; i < 8; i++)
		{// top 노드에서 상하좌우 8방향으로 탐색
			int nextX = x + DX[i];
			int nextZ = z + DZ[i];
			//지정된 범위를 벗어나지 않게
			if (nextX >= 0 && nextX < m_vecMap.size() && nextZ >= 0 && nextZ < m_vecMap.size())
			{
				//막힌길이거나 내가 온 길이면 다시 못오게
				if (m_vecMap[nextZ][nextX] != 1 && m_vecMap[nextZ][nextX] == false)
				{
					tAddNode.x = nextX;
					tAddNode.z = nextZ;
					tAddNode.iCurCos = i < 4 ? cost + 10 : cost + 14;// 상하좌우면 10, 대각선이면 14(√200)
					tAddNode.iGoalCos = (abs(_tGoal.second - tAddNode.x) + abs(_tGoal.first - tAddNode.z)) * 10;
					tAddNode.pParentNode = make_pair(z, x);	// 기존 top노드를 부모 노드로 설정
					m_vecClose[nextZ][nextX] = true;		// 내가 간 길은 true해서 다시 못오게
					m_vecMap[nextZ][nextX] = 9;		// 탐색한 주변 노드 = (아직 가지 않았으니깐 9로 표시)
					queueWay.push(tAddNode);				// 우선순위 큐에 삽입	
				}
			}
		}
	}

	//이때까지 온 길을 역순으로 가져옵니다
	int px = vecWay.back().x;
	int pz = vecWay.back().z;
	//마지막 하나 남겨두기
	while (vecWay.size() <= 1) 
	{
		if (px == vecWay.back().x && pz == vecWay.back().z) {// 목표 노드부터 부모 노드를 탐색해 역추적
			m_vecMap[pz][px] = 4;//역으로 온 길은 4로 기록해둡니다
			//역으로 가져온 노드의 부모노드(내가 왔던 길)의 xy값을가져와서 현재 노드로 설정합니다
			px = vecWay.back().pParentNode.second;
			pz = vecWay.back().pParentNode.first;
		}
		vecWay.pop_back(); //내가 봤던 길은 삭제합니다 (다음 노드를 확인하기 위해서)
	}
	
	tNode tDir = vecWay.back();
	m_mapNavInfo.find(_ID)->second.vSearchFoce = Vec3(tDir.x, 0.f, tDir.z);
}

void CNavMeshMgr::init_target(int _ID, const Vec3& _vStartPos, const Vec3& _vGoalPos)
{
	map<int, tNavInfo>::iterator iter
		= m_mapNavInfo.find(_ID);

	if (iter == m_mapNavInfo.end())
		return;

	iter->second.bSearchOn = true;
	iter->second.vCurPos = _vStartPos;
	iter->second.vGoalPos = _vGoalPos;
}


void CNavMeshMgr::tick()
{
	map<int, tNavInfo>::iterator iter
		= m_mapNavInfo.begin();

	for (iter; iter != m_mapNavInfo.end(); ++iter)
	{
		if (!iter->second.bSearchOn)
			continue;

		iter->second.fCurTime += DT;
		if (iter->second.fCurTime > iter->second.fEndTime)
		{
			iter->second.fCurTime = 0.f;
		}
	}
}


void CNavMeshMgr::Init_Map()
{
	const vector<CGameObject*>& vecLand =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::LandScape)->GetObjects();

	if (vecLand.size() == 0)
		return;
	//일단 가장 처음 지형만 탐색
	CLandScape* pScape = vecLand[0]->LandScape();
	Matrix matScape = pScape->GetOwner()->Collider3D()->GetColliderWorldMat();
	Vec3 vScapePos = Vec3(matScape._41, matScape._42, matScape._43); //9000, 9000
	Vec3 vScale = pScape->GetOwner()->Collider3D()->GetOffsetScale(); // 18000 18000
	vScale /= 2.f;

	Vec3 vRightTop = vScapePos + vScale;
	Vec3 vLeftDown = vScapePos - vScale;
	m_vLandScapeLen = vRightTop - vLeftDown;
	//Vec3 vCapeLen 
	//40 X 40 2차원 배열
	m_iDivideX = 80; //전체 지형을 나눌 수
	m_iDivideZ = 80;

	m_vecMap.resize((int)m_iDivideZ);
	m_vecClose.resize((int)m_iDivideZ);
	for (int i = 0; i < (int)m_iDivideZ; ++i)
	{
		m_vecMap[i].resize(m_iDivideX);
		m_vecClose[i].resize(m_iDivideX);
	}


	//--- 지형 못가는 쪽 생성
	Init_CloseMap();
}

void CNavMeshMgr::Init_CloseMap()
{
	const vector<CGameObject*>& vecLandform =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::landform)->GetObjects();

	for (int i = 0; i < vecLandform.size(); ++i)
	{
		if (!vecLandform[i]->Collider3D())
			continue;

		Matrix matForm = vecLandform[i]->Collider3D()->GetColliderWorldMat();
		Vec3 vformPos = Vec3(matForm._41, matForm._42, matForm._43);

		//크기
		Vec3 vformScale = vecLandform[i]->Collider3D()->GetOffsetScale();
		vformScale /= 2.f;
		//좌 우 하 상
		float fPos[4] = { vformPos.x - vformScale.x, vformPos.x + vformScale.x,
						 vformPos.z - vformScale.z, vformPos.z + vformScale.z };

		//칸당 갈수있는 길이
		float fOffsetX = m_vLandScapeLen.x / m_iDivideX;
		float fOffsetZ = m_vLandScapeLen.z / m_iDivideZ;

		for (float i = fPos[2]; i <= fPos[3]; i += fOffsetZ)//z
		{
			for (float j = fPos[0]; j <= fPos[1]; j += fOffsetX) //x
			{
				//현재 검사하는 위치
				Vec3 vPos = Vec3{ j, 0.f, i };

				//맵에서의 비율을 잡기
				Vec3 vRatio = vPos / m_vLandScapeLen;
				//Vec3 vAbsRatio = Vec3(abs(vRatio.x), abs(vRatio.y), abs(vRatio.z));

				//맵에서의 비율을 잡기
				vRatio.x *= m_iDivideX; //0.5 * 40 = 20
				vRatio.z *= m_iDivideZ;
				//못가는 길
				m_vecClose[(int)vRatio.x][(int)vRatio.z] = 1;
			}
		}
	}
}

void CNavMeshMgr::Tracking(const Vec3& _vStartPos, const Vec3& _vGoalPos)
{

}

void CNavMeshMgr::TrackingPlayer(CGameObject* _pStartObj)
{
	const vector<CGameObject*>& vecPlayer =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetObjects();

	CGameObject* pPlayer = vecPlayer[0];
	if (pPlayer->IsDead())
		return;

	Matrix matPlayerPos = pPlayer->Collider3D()->GetColliderWorldMat();
	//플레이어 현재 몬스터 위치 비율로 잡아서 맵에 넣기
	Vec3 vPlayerPos = Vec3(matPlayerPos._41, matPlayerPos._42, matPlayerPos._43); 

	//y <----> z 반전
	Vec3 vRatio = vPlayerPos / m_vLandScapeLen;
	vRatio.x *= m_iDivideX; 
	vRatio.y *= m_iDivideZ;
	
	m_vecMap[(int)vRatio.x][(int)vRatio.z] = 5;//목적지는 5
	Vec3 vGoalPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);
	// --몬스터

	Matrix matStartPos = _pStartObj->Collider3D()->GetColliderWorldMat();
	Vec3 vMonsterPos = Vec3(matStartPos._41, matStartPos._42, matStartPos._43); 

	//y <----> z 반전
	vRatio = vMonsterPos / m_vLandScapeLen;
	vRatio.x *= m_iDivideX;
	vRatio.y *= m_iDivideZ;
	m_vecMap[(int)vRatio.x][(int)vRatio.z] = 6;//시작 위치는 6	
	Vec3 vStartPos = Vec3((int)vRatio.x, 0.f, (int)vRatio.z);

	int ID = _pStartObj->GetID();
	init_target(ID, vStartPos, vGoalPos);

	pair<int, int> tStart = make_pair(vStartPos.z, vStartPos.x);
	pair<int, int> tGoal = make_pair(vGoalPos.z, vGoalPos.x);

	//길찾기
	astar(ID, tStart, tGoal);
}


Vec3 CNavMeshMgr::GetTargetPath(CGameObject* _pObj)
{
	int ID = _pObj->GetID();
	vector<std::pair<int, int>> vecFinalPath = {};

	map<int, tNavInfo>::iterator iter =
		m_mapNavInfo.find(ID);

	return iter->second.vSearchFoce;
}
