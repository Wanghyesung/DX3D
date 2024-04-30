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

