#include "pch.h"
#include "CJumpState.h"
#include "CAnimation3D.h"
#include "CStructuredBuffer.h"
#include "CKeyMgr.h"
#include "CPxRigidbody.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CCollider3D.h"
#include "CFSM.h"
#include "CJumpingState.h"
CJumpState::CJumpState() :
	m_iJumpFrame(938),
	m_iEndFrame(953),
	m_vJumpVel(Vec3(0.f, 1000.f,0.f)),
	m_iBoneIdx(32),
	m_vBeginOffsetPos(Vec3::Zero)
{
	
}

CJumpState::~CJumpState()
{

}

void CJumpState::final_tick()
{
	CGameObject* pChar = GetOwner();
	CAnimator3D* pAnim = pChar->GetChild().at(0)->Animator3D();
	vector<Matrix> matJumpBone = {};
	matJumpBone.resize(pAnim->GetBoneCount());
	pAnim->GetFinalBoneMat()->GetData(matJumpBone.data());

	Matrix matJumBone = matJumpBone[m_iBoneIdx];
	matJumBone.m[3][3] = 1;
	matJumBone = XMMatrixTranspose(matJumBone);

	Matrix matCharWorldScale = pChar->Transform()->GetWorldScaleMat();// 캐릭터 크기 역행렬
	Matrix matCharSclaeInv = XMMatrixInverse(nullptr, matCharWorldScale);
	Matrix matCharWorld = pChar->Transform()->GetWorldMat(); // 캐릭터 월드
	Matrix matFinal =  matCharSclaeInv * matJumBone * matCharWorld;

	//1
	float fDiff = matFinal._42 - matCharWorld._42 - m_vBeginOffsetPos.y;
	GetOwner()->PxRigidbody()->SetOffsetPosition(Vec3(0.f, -fDiff , 0.f));

	
	UINT iFrame = pAnim->GetCurAnim()->GetCurFrame();
	addForce(iFrame);
	
}

void CJumpState::Enter()
{
	wstring strFinalAnim = GetName();
	Chanage_Anim(strFinalAnim, false);

	GetOwner()->PxRigidbody()->ChanageMaterial(10.f, 0.f);
	GetOwner()->PxRigidbody()->SetGround(false);
	//이거 이벤트 처리로 일정 순간에 ground false인걸로 바꾸기
	//GetOwner()->PxRigidbody()->ClearFoce(); 
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eIMPULSE);//폭발적으로 움직일 떄
	//
	
	m_vMaxPos = GetOwner()->PxRigidbody()->GetPxPosition();
	m_vMaxPos += Vec3(0.f, 800.f, 0.f);
	m_vJumpVel = Vec3(0.f, 1000.f, 0.f);

	m_vBeginOffsetPos = GetOwner()->PxRigidbody()->GetOffsetPosition();

	m_bAttack = false;
}

void CJumpState::Exit()
{
	GetOwner()->PxRigidbody()->SetOffsetPosition(m_vBeginOffsetPos);

	GetOwner()->PxRigidbody()->ChanageMaterial(0.f, 0.f);
}

void CJumpState::addForce(UINT _iFrame)
{
	if (_iFrame < 924)
		return;

	check_attack();

	Vec3 vFinalVel = {};
	check_key(vFinalVel);

	//위로
	if (_iFrame <= m_iJumpFrame)
	{
		if (check_pos())
			GetOwner()->PxRigidbody()->AddForce(vFinalVel + m_vJumpVel);
	}

	//아래로
	else
	{
		Vec3 vForce = vFinalVel + -m_vJumpVel * 2.8f;

		if (!check_pos())
			GetOwner()->PxRigidbody()->AddForce(vForce);

		if (_iFrame >= m_iEndFrame)
		{
			ChanageState(GetFSM(), STATE_TYPE::JUMPING);
			CJumpingState* pJumping = GetFSM()->GetState<CJumpingState>();
			pJumping->SetFinalFoce(vForce);
			return;
		}
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

void CJumpState::check_key(Vec3& _vFinalVel)
{
	CGameObject* pObj = GetOwner();
	Vec3 vFoward = pObj->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = pObj->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = 120.f;

	if (KEY_PRESSED(KEY::D))
	{
		vRight *= fSpeed;
		_vFinalVel += vRight;
	}
	else if (KEY_PRESSED(KEY::A))
	{
		vRight *= fSpeed;
		_vFinalVel -= vRight;
	}

	if (KEY_PRESSED(KEY::W))
	{
		vFoward *= fSpeed;
		_vFinalVel -= vFoward;

	}
	else if (KEY_PRESSED(KEY::S))
	{
		vFoward *= fSpeed;
		_vFinalVel += vFoward;
	}
}

void CJumpState::check_attack()
{
	if(!m_bAttack && KEY_TAP(KEY::LBTN))
	{
		m_bAttack = true;
	}
}



