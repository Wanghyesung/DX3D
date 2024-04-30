#include "pch.h"
#include "CDemonIdle.h"

#include "CGameObject.h"
#include "CCollider3D.h"
#include "CTransform.h"
CDemonIdle::CDemonIdle()
{
	SetStopLen(1000.f);
}

CDemonIdle::~CDemonIdle()
{

}


void CDemonIdle::final_tick()
{
	if (m_pTarget == nullptr || m_pTarget->IsDead())
		return;

	check_player();
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

	float fCheckLen = 3000.f;
	float fStopLen = GetStopLen();//500

	if (fLen <= fCheckLen)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::WALK);
	}

	else if (fLen <= fStopLen)
	{
		//각도 계산 캐릭터가 위에 있는지
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::ATTACK);//wait
	}
}

