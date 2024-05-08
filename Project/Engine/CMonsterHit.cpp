#include "pch.h"
#include "CMonsterHit.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CRigidbody.h"
#include "CTransform.h"
#include "CPxRigidbody.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"
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
	
	m_fCurFoceTime += DT;

	if (m_fCurFoceTime <= m_tHitInfo.fHitTime)
	{
		CGameObject* pObj = GetOwner();

		Vec3 vFront = pObj->Transform()->GetRelativeDir(DIR_TYPE::UP);
		vFront.y = 0.f;
		Vec3 vFoce = vFront * m_tHitInfo.fHitRcnt;
		float fRatio = (m_tHitInfo.fHitTime / m_fCurFoceTime);

		m_vFinalFoce = vFoce * fRatio;

		GetOwner()->PxRigidbody()->AddVelocity(m_vFinalFoce);
	}
}

void CMonsterHit::Exit()
{
	
}

void CMonsterHit::Enter()
{
	m_fCurFoceTime = 0.f;

	if (m_tHitInfo.bDown)
	{
		Chanage_Anim(GetName()+ L"Down", false);
	}
	else
	{
		Chanage_Anim(GetName(), true);
	}

	//CFontMgr::GetInst()->DrawFont(L"12", 10, 10, 100, 20);
}
