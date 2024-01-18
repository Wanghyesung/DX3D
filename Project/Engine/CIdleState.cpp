#include "pch.h"
#include "CIdleState.h"
#include "CAnimator3D.h"

#include "CKeyMgr.h"
#include "CFSM.h"

void CIdleState::final_tick()
{
	if (KEY_TAP(KEY::A))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Left");
	}
	else if (KEY_TAP(KEY::S))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Back");
	}
	else if (KEY_TAP(KEY::W))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Front");
	}
	else if (KEY_TAP(KEY::D))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
		GetFSM()->SetDir(L"Right");
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
}

void CIdleState::Exit()
{

}

CIdleState::CIdleState()
{

}

CIdleState::~CIdleState()
{

}
