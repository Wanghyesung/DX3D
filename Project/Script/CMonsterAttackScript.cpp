#include "pch.h"
#include "CMonsterAttackScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLayer.h>
#include <Engine\CTimeMgr.h>
CMonsterAttackScript::CMonsterAttackScript():
	CScript((UINT)SCRIPT_TYPE::MONSTERATTACKSCRIPT),
	m_bAttackOn(false)
{

}

CMonsterAttackScript::~CMonsterAttackScript()
{

}


void CMonsterAttackScript::init_attack()
{
	m_bAttackOn = true;
	m_fCurAttackTime = m_fAttackTime;
}

void CMonsterAttackScript::tick()
{
	if (m_bAttackOn)
		return;

	if (m_fAttackTime > m_fCurAttackTime)
	{
		m_fCurAttackTime += DT;
	}

	else if (m_fCurAttackTime >= m_fAttackTime)
	{
		m_bAttackOn = true;
	}
		
}

void CMonsterAttackScript::begin()
{
	const vector<CGameObject*>& vecObj =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();
	if (vecObj[0] != nullptr)
		m_pTarget = vecObj[0];

}


bool CMonsterAttackScript::IsAttackOn()
{
	if (m_bAttackOn)
	{
		m_bAttackOn = false;
		return true;
	}
}

void CMonsterAttackScript::BeginOverlap(CCollider3D* _Other)
{
	
}

void CMonsterAttackScript::OnOverlap(CCollider3D* _Other)
{

}

void CMonsterAttackScript::EndOverlap(CCollider3D* _Other)
{

}
