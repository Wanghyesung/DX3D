#include "pch.h"
#include "CDemonWait.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CLayer.h"
#include "CPxRigidbody.h"
#include "CTransform.h"
CDemonWait::CDemonWait() :
	m_fCurTime(0.f),
	m_fWaitTime(15.f),
	m_fCheckLen(1300.f)
{

}

CDemonWait::~CDemonWait()
{
}


void CDemonWait::final_tick()
{
	m_fCurTime += DT;

	//if (m_fCurTime >= m_fWaitTime)
	//{
	//	ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::JUMP);
	//}
	//
	//if (!check_len())
	//{
	//	ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::WALK);
	//}

	rotate();

}

void CDemonWait::Exit()
{

}

void CDemonWait::Enter()
{
	Chanage_Anim(GetName());

	m_fCurTime = 0.f;
	m_pTarget = 
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);
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

bool CDemonWait::check_len()
{
	Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

	float fLen = (vTargetPos - vPos).Length();
	if (fLen > m_fCheckLen)
		return false;

	return true;
}
