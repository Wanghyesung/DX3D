#pragma once
#include "CSingleton.h"

struct tSpawnInfo
{
	float m_fSpawnTime;
	float m_fCurTime;

	//초기 위치
	Vec3 vInitPos;

	CGameObject* pGameObj;
};

class CRespawnMgr 
	: public CSingleton<CRespawnMgr>
{
	SINGLE(CRespawnMgr)

private:
	vector<tSpawnInfo> m_vecSpawnobj;

public:
	void tick();

	void AddObject(CGameObject* _pGameobj, float _fSpawnTime, Vec3 _vSpanwpos);

private:


};

