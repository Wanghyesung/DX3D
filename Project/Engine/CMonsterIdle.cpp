#include "pch.h"
#include "CMonsterIdle.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CTransform.h"
#include "CCollider3D.h"

CMonsterIdle::CMonsterIdle() :
	m_fCheckLen(1000.f),
	m_fStopLen(110.f)
{

}

CMonsterIdle::~CMonsterIdle()
{

}

void CMonsterIdle::final_tick()
{
	if (m_pTarget == nullptr || m_pTarget->IsDead())
		return;
		
	check_player();
}

void CMonsterIdle::Enter()
{
	Chanage_Anim(GetName());

	m_pTarget = 
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject().at(0);
}

void CMonsterIdle::Exit()
{

}

void CMonsterIdle::check_player()
{
	Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();

	Vec3 vTargetPos = m_pTarget->Collider3D()->GetWorldPos();

	float fLen = (vTargetPos - vPos).Length();

	if (fLen <= m_fCheckLen && m_fStopLen <= fLen)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::RUN);
	}

	else if (m_fStopLen > fLen)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::ATTACK);
	}

}
