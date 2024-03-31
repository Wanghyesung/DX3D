#include "pch.h"
#include "CHitState.h"
#include "CGameObject.h"
#include "CAnimation3D.h"
#include "CTransform.h"
#include "CPxRigidbody.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"
CHitState::CHitState()
{
	
}


CHitState::~CHitState()
{

}

void CHitState::add_foce()
{
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

void CHitState::final_tick()
{
	bool bComplete = GetOwner()->GetChild().at(0)->Animator3D()->GetCurAnim()->IsComplete();
	if (m_bReverse)
	{
		if (bComplete)
		{
			ChanageState(GetFSM(), STATE_TYPE::IDLE);
		}
	}
	else
	{
		if (bComplete)
		{
			m_bReverse = true;
			Chanage_Anim(GetName() + L"_");
		}
			
	}

	add_foce();
	//구간 나누기 reserve , default
}

void CHitState::Exit()
{
	GetOwner()->PxRigidbody()->SetDecrease(false);
}

void CHitState::Enter()
{
	Chanage_Anim(GetName());

	Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);

	Vec3 vDir = vFront * m_tHitInfo.fHitRcnt;

	GetOwner()->PxRigidbody()->SetVelocity(vDir);
	GetOwner()->PxRigidbody()->SetDecrease(true);
	m_bReverse = false;
	m_fCurFoceTime = 0.f;
	m_fDecFoce = m_tHitInfo.fHitRcnt;

	CFontMgr::GetInst()->DrawFont(L"12", 10, 10, 100, 20);
}

