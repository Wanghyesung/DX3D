#include "pch.h"
#include "CDemonJump.h"
#include "CAnimation3D.h"
#include "CPxRigidbody.h"
#include "CTimeMgr.h"
// y = y0 - Vy0 * t- 1/2gt^2
// x = x0 - Vx0 * t

CDemonJump::CDemonJump() :
	m_bAttackTrigger(false),
	m_vJumpForce(Vec3(0.f, 3000.f, 0.f)),
	m_iJumpStartFrame(1727),
	m_vFinalJumpPos(Vec3(1783, 2131, 3460))
{
	//1783 2131 3460
}

CDemonJump::~CDemonJump()
{

}

void CDemonJump::jump()
{
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();
	if (vPos.z <= m_vFinalJumpPos.z)
	{
		GetOwner()->PxRigidbody()->SetAddGravity(false);
		return;
	}
	float fDiff = m_vFinalJumpPos.z - vPos.z;

	//현재 목적지까지 남은 비율
	float fRatio = m_fMaxLen / fDiff;

	Vec3 vFront = Vec3(0.f, 0.f, -1100.f);
	Vec3 vFinalVel = (m_vJumpForce + vFront) * fRatio;

	vFinalVel *= fRatio;

	GetOwner()->PxRigidbody()->SetVelocity(vFinalVel);
}

void CDemonJump::final_tick()
{
	if (m_bAttackTrigger)
	{
		int a = 10;
	}
	 int iFrame =
		 GetOwner()->GetChild().at(0)->Animator3D()->GetCurAnim()->GetCurFrame();

	 if (iFrame >= m_iJumpStartFrame)
	 {
		 jump();
	 }
}

void CDemonJump::Exit()
{
	
}

void CDemonJump::Enter()
{
	Chanage_Anim(GetName(), false);
	m_bAttackTrigger = false;


	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();
	m_fMaxLen = m_vFinalJumpPos.z - vPos.z;
	//GetOwner()->PxRigidbody()->SetGround(false);
	
	//GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eVELOCITY_CHANGE);
}
