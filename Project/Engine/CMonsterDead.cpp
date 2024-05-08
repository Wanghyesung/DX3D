#include "pch.h"
#include "CMonsterDead.h"
#include "CAnimation3D.h"
#include "CPxRigidbody.h"
#include "CCollider3D.h"
#include "CEquip.h"
#include "CPhysxMgr.h"
#include "CRespawnMgr.h"
CMonsterDead::CMonsterDead()
{
	
}

CMonsterDead::~CMonsterDead()
{

}


void CMonsterDead::final_tick()
{
	bool bComplete = GetOwner()->GetChild().at(0)->Animator3D()->GetCurAnim()->IsComplete();

	if (bComplete)
	{
		//DestroyObject(GetOwner());
		//ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
		//GetOwner()->PxRigidbody()->SetPxTransform(Vec3(-2000.f, -2000.f, -2000.f));
		GetOwner()->PxRigidbody()->SetGround(true, false);

		//임시로
		CRespawnMgr::GetInst()->AddObject(GetOwner(), 10.f, Vec3(2000.f, 500.f, 2000.f));
	}
}

void CMonsterDead::Exit()
{

}

void CMonsterDead::Enter()
{
	Chanage_Anim(GetName(),false);

	//CPhysxMgr::GetInst()->DeleteEventObj(GetOwner()->GetID());
	GetOwner()->PxRigidbody()->SetVelocity(Vec3::Zero);

	//이거 끄고 m_mapcoll좀 어떻게 해야함
	//GetOwner()->Collider3D()->SetActive(false);
}

