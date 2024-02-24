#include "pch.h"
#include "CJumpState.h"
#include "CAnimation3D.h"
#include "CStructuredBuffer.h"
#include "CKeyMgr.h"
#include "CPxRigidbody.h"
#include "CTransform.h"


CJumpState::CJumpState() :
	m_iJumpFrame(1122),
	m_vJumpVel(Vec3(0.f, 2800.f,0.f))
{

}

CJumpState::~CJumpState()
{

}

void CJumpState::final_tick()
{
	if (KEY_TAP(KEY::LBTN))
	{
		m_bAttack = true;
	}

	CGameObject* pChildObj = GetOwner()->GetChild().at(0);

	CAnimation3D* pAnim = pChildObj->Animator3D()->GetCurAnim();

	UINT iFrame = pAnim->GetCurFrame();


	if (iFrame >= m_iJumpFrame)
	{
		addForce(iFrame);
	}
}

void CJumpState::Enter()
{
	wstring strFinalAnim = GetName();
	Chanage_Anim(strFinalAnim, false);

	//���߿� y�ุ �̵����ϰ� ���°ɷ� ����
	GetOwner()->PxRigidbody()->BlockTransform(true);//px��ġ�θ� �̵�
	

	m_vMaxPos = GetOwner()->PxRigidbody()->GetPxPosition();
	m_vMaxPos += Vec3(0.f, 250.f, 0.f);
}

void CJumpState::Exit()
{
	m_bAttack = false;

	GetOwner()->PxRigidbody()->BlockTransform(false);
}

void CJumpState::addForce(UINT _iFrame)
{
	//����
	if (_iFrame <= 1134)
	{
		if (check_pos())
			GetOwner()->PxRigidbody()->AddVelocity(m_vJumpVel);

		GetOwner()->PxRigidbody()->SetGround(false);
	}

	//�Ʒ���
	else
	{
		//üũ
		if (GetOwner()->PxRigidbody()->IsGround())
		{
			ChanageState(GetFSM(), STATE_TYPE::JUMPEND);
			return;
		}

		GetOwner()->PxRigidbody()->AddVelocity(-m_vJumpVel / 1.5f);
	}
}

bool CJumpState::check_pos()
{
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();
	if (vPos.y > m_vMaxPos.y)
	{
		vPos.y = m_vMaxPos.y;

		GetOwner()->PxRigidbody()->SetPxTransform(vPos);

		return false;
	}

	return true;
}



