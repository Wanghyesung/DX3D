#include "pch.h"
#include "CDemonHold.h"
#include "CRigidbody.h"
#include "CTimeMgr.h"
#include "CTransform.h"
#include "CCollider3D.h"
#include "CLevelMgr.h"
#include "CLayer.h"
CDemonHold::CDemonHold():
	m_fAttackTime(7.f),
	m_fCheckTime(0.5f),
	m_fAttackCurTime(0.f),
	m_fCheckCurTime(0.f)
{

}

CDemonHold::~CDemonHold()
{

}

void CDemonHold::final_tick()
{
	if (m_pTarget == nullptr || m_pTarget->IsDead())
		return;

	m_fAttackCurTime += DT;
	m_fCheckCurTime += DT;

	if (m_fAttackTime >= m_fAttackCurTime)
	{
		attack();
	}

	else if (m_fCheckTime >= m_fCheckCurTime)
	{
		check_player();
	}
}

void CDemonHold::Enter()
{
	GetOwner()->Rigidbody()->SetGround(true);

	m_pTarget = 
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);
}

void CDemonHold::Exit()
{
	m_fAttackCurTime = 0.f;
	m_fCheckCurTime = 0.f;
}

void CDemonHold::check_player()
{
	//내려갔는지 확인
	Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();
	Vec3 vTargetPos = m_pTarget->Collider3D()->GetWorldPos();
	
	Vec3 vDiff = vPos - vTargetPos;
	//플레이어가 아래
	if (vDiff.y < 0.f)
	{
		//jump로	
	}
}

void CDemonHold::attack()
{

}

