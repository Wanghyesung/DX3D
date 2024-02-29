#include "pch.h"
#include "CWalkState.h"
#include "CKeyMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRigidbody.h"
#include "CPxRigidbody.h"
#include "CFSM.h"
void CWalkState::final_tick()
{
	CGameObject* pObj = GetOwner();
	CPxRigidbody* pRigidbody = pObj->PxRigidbody();

	//z <--> y fbx축 
	Vec3 vFoward = pObj->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vUp = pObj->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = pObj->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = 1500.f;
	Vec3 vForce = Vec3::Zero;

	wstring strDir = {};
	
	if (KEY_PRESSED(KEY::D))
	{
		vRight *= fSpeed;

		vForce += vRight;

		strDir = L"Right";
	}
	else if (KEY_PRESSED(KEY::A))
	{
		vRight *= fSpeed;

		vForce -= vRight;

		strDir = L"Left";
	}

	if (KEY_PRESSED(KEY::W))
	{
		vFoward *= fSpeed;

		vForce -= vFoward;

		strDir = L"Front";
	}
	else if (KEY_PRESSED(KEY::S))
	{
		vFoward *= fSpeed;

		vForce += vFoward;

		strDir = L"Back";
	}

	if (KEY_TAP(KEY::LSHIFT))
	{
		ChanageState(GetFSM(), STATE_TYPE::JUMP);
	}

	else if (KEY_TAP(KEY::SPACE))
	{
		ChanageState(GetFSM(), STATE_TYPE::ROLL);
		return;
	}

	else if (KEY_PRESSED(KEY::SPACE))
	{
		ChanageState(GetFSM(), STATE_TYPE::RUN);
		return;
	}

	if (vForce == Vec3::Zero)
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

	if (GetFSM()->GetDir() != strDir)
	{
		GetFSM()->SetDir(strDir);
		Enter();
	}

	pRigidbody->AddVelocity(vForce);
}

void CWalkState::Enter()
{
	wstring strFinalName = GetName() + GetFSM()->GetDir();

	Chanage_Anim(strFinalName);
}

void CWalkState::Exit()
{

}

CWalkState::CWalkState()
{

}

CWalkState::~CWalkState()
{
}
