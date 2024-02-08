#include "pch.h"
#include "CPlayerHit.h"

#include "CRigidbody.h"
#include "CTransform.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"

CPlayerHit::CPlayerHit():
	m_tHitInfo{}
{
}

CPlayerHit::~CPlayerHit()
{

}

void CPlayerHit::final_tick()
{
	CGameObject* pObj = GetOwner()->GetChild().at(0);
	bool bCompplete = pObj->Animator3D()->GetCurAnim()->IsComplete();

	if (bCompplete)
		ChanageState(GetFSM(), STATE_TYPE::IDLE);
}

void CPlayerHit::Exit()
{
	CRigidbody* pRigidbody = GetOwner()->Rigidbody();

	pRigidbody->SetAcumulate(false);
	pRigidbody->SetFricoeff(false);
}

void CPlayerHit::Enter()
{
	if (m_tHitInfo.bDown)
	{
		Chanage_Anim(GetName() + L"Down", false);
	}
	else
	{
		Chanage_Anim(GetName(), false);
	}

	CRigidbody* pRigidbody = GetOwner()->Rigidbody();
	pRigidbody->SetAcumulate(true);
	pRigidbody->SetFricoeff(true);

	Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);

	Vec3 vDir = vFront * m_tHitInfo.fHitRcnt;

	pRigidbody->SetVelocity(vDir);
}

