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

class CNavMeshMgr :
	public CSingleton<CNavMeshMgr>
{
	SINGLE(CNavMeshMgr);

private:
	//현재 내 맵
	vector<vector<int>> m_vecMap;
	vector<vector<int>> m_vecClose;//닫힌 곳

	//몬스터마다 경로탐색에 필요한 시간
	map<int, tNavInfo> m_mapInitTime;

private:
	void add_monster(CGameObject* pGameObj, float _fSearchTime);

	void astar();
public:
	void tick();

	//현재 맵에 따라 맵벡터 초기화 (열린길, 막힌길 ..)
	void Init_Map();
	void Init_Close();

	//현재 내 위치에서 목적지까지의 최단경로
	const vector<std::pair<int, int>>& GetTargetPath(int _iID, const Vec3& _vecStartPos);
	
};

