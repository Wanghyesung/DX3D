#include "pch.h"
#include "CDemonIdle.h"

#include "CGameObject.h"
#include "CCollider3D.h"
CDemonIdle::CDemonIdle()
{

}

CDemonIdle::~CDemonIdle()
{

}


void CDemonIdle::final_tick()
{
	if (m_pTarget == nullptr || m_pTarget->IsDead())
		return;

	//check_player();
}

void CDemonIdle::Enter()
{
	CMonsterIdle::Enter();
}


void CDemonIdle::Exit()
{

}

void CDemonIdle::check_player()
{
	Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();

	Vec3 vTargetPos = m_pTarget->Collider3D()->GetWorldPos();

	float fLen = (vTargetPos - vPos).Length();

	float fCheckLen = GetCheckLen();
	float fStopLen = GetStopLen();

	if (fLen <= fCheckLen && fStopLen <= fLen)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::WALK);
	}

	else if (fStopLen > fLen)
	{
		//각도 계산 캐릭터가 위에 있는지
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::ATTACK);
	}
}

