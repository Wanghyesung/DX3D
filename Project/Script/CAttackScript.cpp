#include "CAttackScript.h"
#include "pch.h"
#include "CAttackScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CObjstacleScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CAttackState.h>

CAttackScript::CAttackScript() :
	CScript((UINT)SCRIPT_TYPE::ATTACKSCRIPT)
{
}

CAttackScript::CAttackScript(SCRIPT_TYPE _eType):
	CScript((UINT)_eType)
{
}

CAttackScript::~CAttackScript()
{

}

void CAttackScript::add_monster(int _ID)
{
	vector< tCheckInfo>::iterator iter = m_vecCheck.begin();
	for (iter; iter != m_vecCheck.end(); ++iter)
	{
		if (iter->ID == _ID)
			return;
	}

	//ù ����
	tCheckInfo tInfo = {};
	tInfo.ID = _ID;
	tInfo.fAttackTime = m_tAttack.fAttackTime;
	tInfo.fCurTime = 0.f;
	//tInfo.iAttackCount = 1;
	tInfo.bAttackOn = true;
	
	if (m_vecCheck.size() >= m_tAttack.iMaxCount)
		return;

	m_vecCheck.push_back(tInfo);
}

void CAttackScript::erase_monster(int _ID)
{
	vector< tCheckInfo>::iterator iter = m_vecCheck.begin();
	for (iter; iter != m_vecCheck.end(); ++iter)
	{
		if (iter->ID == _ID)
		{
			m_vecCheck.erase(iter);
			return;
		}
	}
}

tCheckInfo& CAttackScript::find_monster(int _ID)
{
	vector< tCheckInfo>::iterator iter = m_vecCheck.begin();
	for (iter; iter != m_vecCheck.end(); ++iter)
	{
		if (iter->ID == _ID)
		{
			return *iter;
		}
			
	}
	tCheckInfo tNULL = {};

	return tNULL;
}

void CAttackScript::tick()
{
	//vector< tCheckInfo>::iterator iter = m_vecCheck.begin();
	//
	//for (iter; iter != m_vecCheck.end(); ++iter)
	//{
	//	if (iter->bAttackOn)
	//		continue;
	//
	//	iter->fCurTime += DT;
	//	if (iter->fCurTime >= iter->fAttackTime)
	//	{
	//		iter->bAttackOn = true;
	//		iter->fCurTime = 0.f;
	//	}
	//}	

}

void CAttackScript::begin()
{
	m_vecCheck.clear();
}

void CAttackScript::BeginOverlap(CCollider3D* _Other)
{
	CMonsterScript* pMonsterScript = _Other->GetOwner()->GetScript<CMonsterScript>();
	if (pMonsterScript)
	{
		CGameObject* pOtherObj = _Other->GetOwner();
		add_monster(pOtherObj->GetID());

		tHitInfo tHit = {};
		tHit.bDown = m_tAttack.bDown;
		tHit.fHitRcnt = m_tAttack.fAttRcnt;
		tHit.fDamage = m_tAttack.fDamage;
		tHit.fHitTime = m_tAttack.fAddForceTime;
		tHit.vHitPos = pOtherObj->PxRigidbody()->GetPxPosition();

		pMonsterScript->SetHitInfo(tHit);
	}
}

void CAttackScript::OnOverlap(CCollider3D* _Other)
{	
	int a = 10;

	if (_Other->GetOwner()->GetScript<CMonsterScript>())
	{
		int a = 10;
	}
}

void CAttackScript::EndOverlap(CCollider3D* _Other)
{
	CMonsterScript* pMonsterScript = _Other->GetOwner()->GetScript<CMonsterScript>();
	if (pMonsterScript)
	{
		erase_monster(_Other->GetOwner()->GetID());
	}
}

bool CAttackScript::IsAttackOn(int _ID)
{
	tCheckInfo& tInfo = find_monster(_ID);
	if(tInfo.ID == -1)
		return false;

	if (tInfo.bAttackOn)
	{
		tInfo.bAttackOn = false;
		//++tInfo.iAttackCount;
		return true;
	}

	return false;
}
