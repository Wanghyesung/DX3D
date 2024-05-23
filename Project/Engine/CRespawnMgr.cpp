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
	//���� ���Ϳ� ���� ������Ʈ���� Level vector�� ���� ������ ���⼭ �޸� ����
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

			//physxRigidbody���� �׳� rigidbody���� Ȯ���ϱ�

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
