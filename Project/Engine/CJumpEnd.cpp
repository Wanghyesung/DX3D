#include "pch.h"
#include "CJumpEnd.h"
#include "CAnimation3D.h"
#include "CPxRigidbody.h"
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

	wstring strFinalAnim = GetName();
	Chanage_Anim(strFinalAnim, false);
}

void CJumpEnd::Exit()
{
	//GetOwner()->PxRigidbody()->SetGround(false);
}



