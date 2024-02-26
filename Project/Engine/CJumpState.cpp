#include "pch.h"
#include "CJumpState.h"
#include "CAnimation3D.h"
#include "CStructuredBuffer.h"
#include "CKeyMgr.h"
#include "CPxRigidbody.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CJumpState::CJumpState() :
	m_iJumpFrame(0),
	m_vJumpVel(Vec3(0.f, 1000.f,0.f))
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

	addForce(iFrame);
	
}

void CJumpState::Enter()
{
	wstring strFinalAnim = GetName();
	Chanage_Anim(strFinalAnim, false);

	//���߿� y�ุ �̵����ϰ� ���°ɷ� ����
	GetOwner()->PxRigidbody()->BlockTransform(true);//px��ġ�θ� �̵�
	GetOwner()->PxRigidbody()->SetGround(false);
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eIMPULSE);//���������� ������ ��

	
	m_vMaxPos = GetOwner()->PxRigidbody()->GetPxPosition();
	m_vMaxPos += Vec3(0.f, 500.f, 0.f);

	m_vJumpVel = Vec3(0.f, 1000.f, 0.f);
}

void CJumpState::Exit()
{
	m_bAttack = false;

	GetOwner()->PxRigidbody()->BlockTransform(false);
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eACCELERATION);

}

void CJumpState::addForce(UINT _iFrame)
{
	//����
	if (_iFrame <= 937)
	{
		if (check_pos())
			GetOwner()->PxRigidbody()->AddForce(m_vJumpVel);
	}

	//�Ʒ���
	else
	{
		//üũ
		//���� �����ϸ�
		if (GetOwner()->PxRigidbody()->IsGround())
		{
			ChanageState(GetFSM(), STATE_TYPE::JUMPEND);
			return;
		}
	
		if(!check_pos())
			GetOwner()->PxRigidbody()->AddForce(-m_vJumpVel * 3.2f);
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



