#pragma once
#include "CSingleton.h"

struct tNavInfo
{
	int iID;

	float fCurTime;
	float fEndTime;

	Vec3 vCurPos;
	Vec3 vGoalPos;

	//갈 방향
	Vec3 vSearchFoce;

	//탐색필요시간이 되었는지
	bool bSearchOn;
};

struct tNode
{
	int x, z;//좌표
	int iCurCos, iGoalCos;//시작부터 현재까지 비용, 현재부터 목적지까지의 최종 비용 
	pair<int, int> pParentNode;//역추적에 쓰일 이전 노드
};

//여기서 위치 기준은 전부 collider로 잡습니다
class CNavMeshMgr :
	public CSingleton<CNavMeshMgr>
{
	SINGLE(CNavMeshMgr);

private:
	//현재 내 맵
	vector<vector<int>> m_vecMap;
	vector<vector<int>> m_vecClose;//닫힌 곳

	//몬스터마다 경로탐색에 필요한 시간
	map<int, tNavInfo> m_mapNavInfo;

	Vec3 m_vLandScapeLen;//내 맵 전체 길이
	int  m_iDivideX; //맵을 나눌 수
	int  m_iDivideZ;
private:
	void init_target(int _ID, const Vec3& _vStartPos , const Vec3& _vGoalPos);
	void astar(int _ID, pair<int, int> start, pair<int, int> goal);
public:
	void tick();

	//현재 맵에 따라 맵벡터 초기화 (열린길, 막힌길 ..)
	void Init_Map();
	void Init_CloseMap();

	void AddMonster(CGameObject* pGameObj, float _fSearchTime);
	void DeleteMonster(CGameObject* pGameObj);

	void Tracking(const Vec3& _vStartPos, const Vec3& _vGoalPos);
	void TrackingPlayer(CGameObject* _pStartObj);

	
	Vec3 GetTargetPath(CGameObject* _pObj);
	
};

