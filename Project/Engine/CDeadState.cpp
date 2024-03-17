#include "pch.h"
#include "CDeadState.h"
#include "CGameObject.h"
#include "CAnimation3D.h"

CDeadState::CDeadState()
{

}

CDeadState::~CDeadState()
{

}



void CDeadState::final_tick()
{
	bool bComplete = GetOwner()->GetChild().at(0)->Animator3D()->GetCurAnim()->IsComplete();

	if (bComplete)
	{
		dead();

		ChanageState(GetFSM(), STATE_TYPE::IDLE);
	}

}

void CDeadState::Exit()
{

}

void CDeadState::Enter()
{
	Chanage_Anim(GetName());

	//충돌체 끄기
}

void CDeadState::dead()
{
	//물체 옮기기 상태는 dead하지말고
}

