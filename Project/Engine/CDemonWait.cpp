#include "pch.h"
#include "CDemonWait.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CLayer.h"
#include "CPxRigidbody.h"
#include "CTransform.h"
#include "CAnimator3D.h"
CDemonWait::CDemonWait() :
	m_fCurTime(0.f),
	m_fWaitTime(5.f),
	m_fCheckLen(1200.f),
	m_fMoveLen(1000.f),
	m_bJumpOn(false)
{

}

CDemonWait::~CDemonWait()
{
}


void CDemonWait::final_tick()
{
	if (!m_bJumpOn)
	{
		move_back();
	}
	else
	{
		m_fCurTime += DT;
		if (m_fCurTime >= m_fWaitTime)
		{
			ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::JUMP);
		}
	}

}

void CDemonWait::Exit()
{

}

void CDemonWait::Enter()
{
    Chanage_Anim(L"Walk_Back");

	m_bJumpOn = false;
	m_fStartLen = GetOwner()->PxRigidbody()->GetPxPosition().z;
	m_fCurTime = 0.f;
	m_pTarget = 
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);


	Vec3 vFinalRot = Vec3(-XM_PI / 2.f, XM_PI, 0.f);

	PxQuat yRotation(vFinalRot.y, PxVec3(0.0f, 1.0f, 0.0f));
	GetOwner()->PxRigidbody()->SetPxRotate(yRotation);
	GetOwner()->Transform()->SetRelativeRot(vFinalRot);
}

void CDemonWait::rotate()
{
	//z <--> y fbx축 
	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);
	//vFoward.y = 0.f;

	Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

	Vec3 vDir = (vTargetPos - vPos).Normalize();

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

	//
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	Vec3 vFinalRot = Vec3(-XM_PI / 2.f, fRadian, 0.f);

	PxQuat yRotation(vFinalRot.y, PxVec3(0.0f, 1.0f, 0.0f));
	GetOwner()->PxRigidbody()->SetPxRotate(yRotation);
	GetOwner()->Transform()->SetRelativeRot(vFinalRot);
}

bool CDemonWait::check_dir()
{
	Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

	float fLen = (vTargetPos - vPos).Length();
	if (fLen > m_fCheckLen)
		return false;

	return true;
}

void CDemonWait::move_back()
{
	Vec3 vFront = Vec3(0.f, 0.f, 1.f);

	CPxRigidbody* pRigid = GetOwner()->PxRigidbody();
	pRigid->AddVelocity(vFront * 400.f);
	

	float fCurLen = pRigid->GetPxPosition().z;
	float fLen = fCurLen - m_fStartLen;

	if (fabs(fLen) >= m_fCheckLen)
	{
		m_bJumpOn = true;
		Chanage_Anim(GetName());
	}
	//내 범위에서 벗어났는지 확인
	//check_dir();
}

