#include "pch.h"
#include "CRollState.h"
#include "CFSM.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CRigidbody.h"
#include "CTransform.h"
#include "CKeyMgr.h"
#include "CPxRigidbody.h"
void CRollState::final_tick()
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
	pObj->PxRigidbody()->SetVelocity(m_vFoce);
}

void CRollState::Exit()
{	
	m_vFoce = Vec3::Zero;
}

void CRollState::Enter()
{
	CGameObject* pObj = GetOwner();

	wstring strFinalAnim = GetName() + GetFSM()->GetDir();
	Chanage_Anim(strFinalAnim, false);


	Vec3 vFoward = pObj->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = pObj->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = 300.f;
	Vec3 vForce = Vec3::Zero;

	wstring strDir = GetFSM()->GetDir();

	if (strDir == L"Right")
	{
		vRight *= fSpeed;

		vForce += vRight;
	}
	else if (strDir == L"Left")
	{
		vRight *= fSpeed;

		vForce -= vRight;
	}

	else if (strDir == L"Front")
	{
		vFoward *= fSpeed;

		vForce -= vFoward;
	}
	else if (strDir == L"Back")
	{
		vFoward *= fSpeed;

		vForce += vFoward;
	}

	vForce.y = 0.f;
	m_vFoce = vForce;
}


CRollState::CRollState():
	m_vFoce(Vec3::Zero)
{

}

CRollState::~CRollState()
{
}
