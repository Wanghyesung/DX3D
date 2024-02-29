#include "pch.h"
#include "CJumpState.h"
#include "CAnimation3D.h"
#include "CStructuredBuffer.h"
#include "CKeyMgr.h"
#include "CPxRigidbody.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CCollider3D.h"
CJumpState::CJumpState() :
	m_iJumpFrame(0),
	m_iEndFrame(953),
	m_vJumpVel(Vec3(0.f, 1000.f,0.f)),
	m_iBoneIdx(0)
{
	//49
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
	float fDiff = matFinal._42 - matCharWorld._42;
	GetOwner()->PxRigidbody()->SetOffsetPosition(Vec3(0.f, -fDiff , 0.f));

	//Vec3 vPos = Vec3(matFinal._41, matFinal._42, matFinal._43);
	//GetOwner()->PxRigidbody()->SetPxTransform(vPos);

	//GetOwner()->Collider3D()->SetOffsetPos(Vec3(0.f, -fDiff, 0.f));
	////73
	//
	UINT iFrame = pAnim->GetCurAnim()->GetCurFrame();
	////
	//addForce(iFrame);

	if (iFrame >= m_iEndFrame)
	{
		ChanageState(GetFSM(), STATE_TYPE::IDLE);
	}
	
}

void CJumpState::Enter()
{
	wstring strFinalAnim = GetName();
	Chanage_Anim(strFinalAnim, false);

	//나중에 y축만 이동못하게 막는걸로 수정
	//GetOwner()->PxRigidbody()->BlockTransformY(true);//transformY 위치 이동 막기
	GetOwner()->PxRigidbody()->SetGround(false);
	GetOwner()->PxRigidbody()->SetForceMode(PxForceMode::eIMPULSE);//폭발적으로 움직일 떄

	
	m_vMaxPos = GetOwner()->PxRigidbody()->GetPxPosition();
	m_vMaxPos += Vec3(0.f, 500.f, 0.f);

	m_vJumpVel = Vec3(0.f, 1000.f, 0.f);
}

void CJumpState::Exit()
{
	m_bAttack = false;

	GetOwner()->PxRigidbody()->BlockTransformY(false);
}

void CJumpState::addForce(UINT _iFrame)
{
	//위로
	if (_iFrame <= 938)
	{
		if (check_pos())
			GetOwner()->PxRigidbody()->AddForce(m_vJumpVel);
	}

	//아래로
	else
	{
		//체크
		//땅에 도착하면
		if (_iFrame >= m_iEndFrame)
		{
			ChanageState(GetFSM(), STATE_TYPE::JUMPING);
			return;
		}
	
		if(!check_pos())
			GetOwner()->PxRigidbody()->AddForce(-m_vJumpVel * 2.8f);
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



