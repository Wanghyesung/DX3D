#include "pch.h"
#include "CMonsterIdle.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CTransform.h"

CMonsterIdle::CMonsterIdle() :
	m_fCheckLen(1000.f)
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
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();

	float fLen = (vTargetPos - vPos).Length();

	if (fLen <= m_fCheckLen)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::RUN);
	}
}
