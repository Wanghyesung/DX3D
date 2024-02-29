#include "pch.h"
#include "CJumpingState.h"
#include "CPxRigidbody.h"

CJumpingState::CJumpingState()
{

}

CJumpingState::~CJumpingState()
{

}

void CJumpingState::final_tick()
{
	if (GetOwner()->PxRigidbody()->IsGround())
	{
		ChanageState(GetFSM(), STATE_TYPE::JUMPEND);
	}
}

void CJumpingState::Enter()
{

}

void CJumpingState::Exit()
{
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eACCELERATION); //기본 이동
}

