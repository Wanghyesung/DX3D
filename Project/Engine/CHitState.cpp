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

void CHitState::rotate()
{
	//z <--> y fbx축 
	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);
	//vFoward.y = 0.f;

	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

	Vec3 vDir = (m_tHitInfo.vHitPos - vPos).Normalize();

	//- = 90 ~ 270도 - 두 벡터의 방향이 둔각을 이룬다.
	float fRadian;
	float fCos = vDir.Dot(vFoward);
	//외적 이용 오른쪽 왼쪽 판별
	Vec3 vCross = vFoward.Cross(vDir);

	if (vCross.y >= 0)
	{
		fRadian = XM_PI + acos(fCos);
	}
	else
	{
		fRadian = XM_PI - acos(fCos);
	}

	//Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	//Vec3 vFinalRot = Vec3(-XM_PI / 2.f, fRadian, 0.f);
	//
	//PxQuat yRotation(vFinalRot.y, PxVec3(0.0f, 1.0f, 0.0f));
	//GetOwner()->PxRigidbody()->SetPxRotate(yRotation);
	//GetOwner()->Transform()->SetRelativeRot(vFinalRot);
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

	


	//CFontMgr::GetInst()->DrawFont(L"12", 10, 10, 100, 20);
}

