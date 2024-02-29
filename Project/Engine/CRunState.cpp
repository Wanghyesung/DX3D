#include "pch.h"
#include "CRunState.h"
#include "CKeyMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRigidbody.h"
#include "CPxRigidbody.h"
#include "CFSM.h"
void CRunState::final_tick()
{
	CGameObject* pObj = GetOwner();
	CPxRigidbody* pRigidbody = pObj->PxRigidbody();

	//z <--> y fbx축 
	Vec3 vFoward = pObj->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vUp = pObj->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = pObj->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = 800.f;
	Vec3 vForce = Vec3::Zero;

	wstring strDir = {};

	if (KEY_PRESSED(KEY::W))
	{
		vFoward *= fSpeed;

		vForce -= vFoward;

		strDir = L"Front";
	}

	if (KEY_TAP(KEY::SPACE))
	{
		ChanageState(GetFSM(), STATE_TYPE::ROLL);
		return;
	}

	if (vForce == Vec3::Zero || KEY_RELEASE(KEY::SPACE))
	{
		ChanageState(GetFSM(), STATE_TYPE::IDLE);
		return;
	}

	//foward -> z축 y축 반전
	//up->y축 z축 반전 - 1
	//Vec3 vNewForce = {};
	//vNewForce.x = vForce.x;
	//vNewForce.y = vForce.z;
	//vNewForce.z = vForce.y;

	pRigidbody->AddVelocity(vForce);
}

void CRunState::Enter()
{
	wstring strFinalName = GetName() + GetFSM()->GetDir();

	Chanage_Anim(strFinalName);
}

void CRunState::Exit()
{

}

CRunState::CRunState()
{

}

CRunState::~CRunState()
{
}
