#include "pch.h"
#include "CIdleState.h"
#include "CAnimator3D.h"

#include "CKeyMgr.h"
#include "CFSM.h"
#include "CPxRigidbody.h"
void CIdleState::final_tick()
{
	ChanageState(GetFSM(), STATE_TYPE::JUMP);

	if (KEY_PRESSED(KEY::A))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Left");
	}
	else if (KEY_PRESSED(KEY::S))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Back");
	}
	else if (KEY_PRESSED(KEY::W))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Front");
	}
	else if (KEY_PRESSED(KEY::D))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Right");
	}

	else if (KEY_TAP(KEY::LSHIFT))
	{
		ChanageState(GetFSM(), STATE_TYPE::JUMP);
	}

	if (KEY_TAP(KEY::LBTN))
	{
		ChanageState(GetFSM(), STATE_TYPE::ATTACK);
	}
}

void CIdleState::Enter()
{
	//Idle
	Chanage_Anim(GetName());

	GetOwner()->PxRigidbody()->ChanageMaterial(10.f, 0.f);
}

void CIdleState::Exit()
{
	GetOwner()->PxRigidbody()->ChanageMaterial(0.f, 0.f);
}

CIdleState::CIdleState()
{

}

CIdleState::~CIdleState()
{

}
