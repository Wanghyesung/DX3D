#include "pch.h"
#include "CJumpingState.h"
#include "CPxRigidbody.h"
#include "CFSM.h"
#include "CJumpState.h"
#include "CGameObject.h"
#include "CTransform.h"

CJumpingState::CJumpingState()
{

}

CJumpingState::~CJumpingState()
{
	if (!m_bJumpAttack)
		delete m_pJumpAttackObj;
}

void CJumpingState::final_tick()
{
	CGameObject* pObj = GetOwner();

	if (m_bJumpAttack)
	{
		Vec3 vPos = pObj->PxRigidbody()->GetPxPosition();
		m_pJumpAttackObj->PxRigidbody()->SetPxTransform(vPos);
	}

	if (pObj->PxRigidbody()->IsGround())
	{
		ChanageState(GetFSM(), STATE_TYPE::JUMPEND);

		if (m_bJumpAttack)
			EraseObject(m_pJumpAttackObj, (int)LAYER_TYPE::Attack);
	}
}

void CJumpingState::Enter()
{
	CJumpState* pJump = GetFSM()->GetState<CJumpState>();

	m_bJumpAttack = pJump->IsAttack();
	if (m_bJumpAttack)
	{
		//공격 오브젝트 소환
		SpawnGameObject(m_pJumpAttackObj, Vec3::Zero, (int)LAYER_TYPE::Attack);

		Chanage_Anim(L"Jump_Attack", false);
	}

	else
	{
		Chanage_Anim(L"Jumping", false);
	}
}

void CJumpingState::Exit()
{
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eACCELERATION); //기본 이동
	m_bJumpAttack = false;
}

void CJumpingState::EraseAttack()
{
	EraseObject(m_pJumpAttackObj, (int)LAYER_TYPE::Attack);
	m_pJumpAttackObj->PxRigidbody()->SetPxTransform(Vec3(-2000.f, -2000.f, -2000.f));
 }