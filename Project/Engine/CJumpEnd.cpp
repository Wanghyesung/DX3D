#include "pch.h"
#include "CJumpEnd.h"
#include "CAnimation3D.h"
#include "CPxRigidbody.h"
#include "CJumpState.h"
#include "CFSM.h"
#include "CJumpingState.h"
CJumpEnd::CJumpEnd()
{
}

CJumpEnd::~CJumpEnd()
{

}

void CJumpEnd::final_tick()
{
	CGameObject* pObj = GetOwner();

	//자식 애니메이션으로
	CGameObject* pChildObj = pObj->GetChild().at(0);
	CAnimation3D* pCurAnim = pChildObj->Animator3D()->GetCurAnim();
	bool bComplete = pCurAnim->IsComplete();

	if (bComplete)
	{
		ChanageState(GetFSM(), STATE_TYPE::IDLE);
		return;
	}
}

void CJumpEnd::Enter()
{
	CGameObject* pObj = GetOwner();

	CJumpState* pJump = GetFSM()->GetState<CJumpState>();
	if (pJump->IsAttack())
	{
		Chanage_Anim(L"Jump_Attack_End", false);
	}
	else
	{
		wstring strFinalAnim = GetName();
		Chanage_Anim(strFinalAnim, false);
	}
}

void CJumpEnd::Exit()
{
	CJumpingState* pJumping = GetFSM()->GetState<CJumpingState>();
	pJumping->EraseAttack();
	//GetOwner()->PxRigidbody()->SetGround(false);
}



