#include "pch.h"
#include "CRespawnMgr.h"
#include "CGameObject.h"
#include "CTimeMgr.h"
#include "CPxRigidbody.h"
CRespawnMgr::CRespawnMgr()
{

}

CRespawnMgr::~CRespawnMgr()
{
	//현재 벡터에 들어온 오브젝트들은 Level vector에 없기 떄문에 여기서 메모리 해제
	vector<tSpawnInfo>::iterator iter = m_vecSpawnobj.begin();

	for (iter; iter != m_vecSpawnobj.end(); ++iter)
	{
		delete iter->pGameObj;
		iter->pGameObj = nullptr;
	}
}

void CRespawnMgr::tick()
{
	vector<tSpawnInfo>::iterator iter = m_vecSpawnobj.begin();

	for (iter; iter != m_vecSpawnobj.end(); )
	{
		iter->m_fCurTime += DT;
		if (iter->m_fCurTime >= iter->m_fSpawnTime)
		{
			SpawnGameObject(iter->pGameObj, iter->vInitPos, iter->pGameObj->GetLayerIndex());

			//physxRigidbody인지 그냥 rigidbody인지 확인하기

			iter->pGameObj->PxRigidbody()->SetPxTransform(iter->vInitPos);
			iter = m_vecSpawnobj.erase(iter);

			continue;
		}

		++iter;
	}
}

void CRespawnMgr::AddObject(CGameObject* _pGameobj, float _fSpawnTime, Vec3 _vSpanwpos)
{
	EraseObject(_pGameobj, _pGameobj->GetLayerIndex());
	
	tSpawnInfo tInfo = {};
	tInfo.pGameObj = _pGameobj;
	tInfo.m_fSpawnTime = _fSpawnTime;
	tInfo.vInitPos = _vSpanwpos;

	m_vecSpawnobj.push_back(tInfo);
}
