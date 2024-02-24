#include "pch.h"
#include "CMonsterJump.h"

#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CRigidbody.h"
CMonsterJump::CMonsterJump():
	m_iJumpFrame(100)
{
}

CMonsterJump::~CMonsterJump()
{

}


void CMonsterJump::final_tick()
{
	CAnimator3D* pAnimator = GetOwner()->GetChild().at(0)->Animator3D();
	CAnimation3D* pAnim = pAnimator->GetCurAnim();

	int iFrame = pAnim->GetCurFrame();
	bool bEnd = pAnim->IsComplete();

	if (bEnd)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
		return;
	}

	if (iFrame >= m_iJumpFrame)
	{
		//GetOwner()->Rigidbody()->SetVelocity()
	}

	
}

void CMonsterJump::Enter()
{

}

void CMonsterJump::Exit()
{

}

void CMonsterJump::check_player()
{

}

