#pragma once
#include "CComponent.h"

struct tNode
{
	int x, z;//좌표
	int iCurCos, iGoalCos;//시작부터 현재까지 비용, 현재부터 목적지까지의 최종 비용 
	pair<int, int> pParentNode;//역추적에 쓰일 이전 노드
};


class CNavMesh : public CComponent
{
private:
	float m_fCurTime;
	float m_fTraceTime;

	//현재 내 맵
	vector<vector<int>> m_vecMap;
	vector<vector<int>> m_vecClose;//닫힌 곳

	Vec3 m_vOwnerScale; //몬스터 크기만큼 맵 나누기
	Vec3 m_vLandScapeLen;//내 맵 전체 길이

	Vec3 m_vDivede;
	int  m_iDivideX; //맵 전체 수
	int  m_iDivideZ;

	
	//갈 방향
	Vec3 m_vSearchDir;

	bool m_bActive;

private:
	//void init_target(int _ID);
	void astar(pair<int, int> _tStart, pair<int, int> _tGoal);
	bool aabb(const Vec3& _vGoalNode, const Vec3& _vCurNode);

	void init_map();
	void init_closemap();

	void tracking_player();

public:
	virtual void begin()override;
	virtual void finaltick() override;

	void SetActive(bool _bActive) { m_bActive = _bActive; }
	bool IsActive() { return m_bActive; }

	void Initialize();

	CLONE(CNavMesh);

	Vec3 GetTargetPath() { return m_vSearchDir; }

public:
	virtual void SaveToLevelFile(FILE* _File)override;
	virtual void LoadFromLevelFile(FILE* _FILE)override;

public:
	CNavMesh();
	virtual ~CNavMesh();
};

