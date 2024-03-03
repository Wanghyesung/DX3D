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
		m_pJumpAttackObj->Transform()->SetRelativePos(vPos);
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
		//���� ������Ʈ ��ȯ
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
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eACCELERATION); //�⺻ �̵�

	EraseObject(m_pJumpAttackObj, (int)LAYER_TYPE::Attack);
	m_bJumpAttack = false;
}

