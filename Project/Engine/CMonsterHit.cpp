#include "pch.h"
#include "CMonsterHit.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CRigidbody.h"
#include "CTransform.h"

CMonsterHit::CMonsterHit():
	m_tHitInfo{}
{

}

CMonsterHit::~CMonsterHit()
{

}


void CMonsterHit::final_tick()
{
	CGameObject* pObj = GetOwner()->GetChild().at(0);
	bool bCompplete = pObj->Animator3D()->GetCurAnim()->IsComplete();

	if (bCompplete)
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
}

void CMonsterHit::Exit()
{
	GetOwner()->Rigidbody()->SetAcumulate(false);
	GetOwner()->Rigidbody()->SetFricoeff(false);
}

void CMonsterHit::Enter()
{
	if (m_tHitInfo.bDown)
	{
		Chanage_Anim(GetName()+ L"Down", false);
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
