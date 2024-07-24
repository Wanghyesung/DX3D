#include "pch.h"
#include "CJumpAttackScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CDemonScript.h"

#include <Engine\CAnimator3D.h>
#include <Engine\CPxRigidbody.h>
#include <Engine\CStructuredBuffer.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CFSM.h>
CJumpAttackScript::CJumpAttackScript() :
	CAttackScript(SCRIPT_TYPE::JUMPATTACKSCRIPT),
	m_bBeginOn(false),
	m_pPlayer(nullptr),
	m_iBoneIndex(26),
	m_vOffsetTransform(Vec3(0.f, 540.f, 0.f))
{

}

CJumpAttackScript::~CJumpAttackScript()
{

}

void CJumpAttackScript::check_bone_pos(CGameObject* _pMonster)
{
	CAnimator3D* pAnim = _pMonster->Animator3D();
	CStructuredBuffer* pBoneBuffer = pAnim->GetFinalBoneMat();

	vector<Matrix> vecBone = {};
	vecBone.resize(pAnim->GetBoneCount());
	pBoneBuffer->GetData(vecBone.data());

	m_matFinalBone = vecBone[m_iBoneIndex];
	m_matFinalBone.m[3][3] = 1;
	m_matFinalBone = XMMatrixTranspose(m_matFinalBone);

	Matrix matMonWorldMat = _pMonster->Transform()->GetWorldMat(); //월드 행렬
	Matrix matMonScaleMat = _pMonster->Transform()->GetWorldScaleMat();
	Matrix matMonScaleInv = XMMatrixInverse(nullptr, matMonScaleMat);//크기 역행렬

	Matrix matFinalPos = matMonScaleInv * m_matFinalBone * matMonWorldMat;

	Vec3 vPos = matFinalPos.Translation();
	vPos += m_vOffsetTransform;

	m_pPlayer->PxRigidbody()->SetPxTransform(vPos);

}
bool CJumpAttackScript::check_pos(CGameObject* _pMonster)
{
	if (!_pMonster->GetScript<CDemonScript>())
		return false;

	Vec3 vPlayerPos = m_pPlayer->PxRigidbody()->GetPxPosition();
	Vec3 vMonsterPos = _pMonster->PxRigidbody()->GetPxPosition();

	Vec3 vDiff =  vPlayerPos - vMonsterPos;

	float fRevision = 300.f;

	if (vDiff.y > fRevision)
	{
		//몬스터 up방향 x z 축 곱하기 500
		Vec3 vDir = _pMonster->Transform()->GetRelativeDir(DIR_TYPE::UP);
		vDir.z *= -500.f;
		vDir.x *= -500.f;
		m_vOffsetTransform.z = vDir.z;
		m_vOffsetTransform.x = vDir.x;

		return true;
	}
	return false;
}
void CJumpAttackScript::lookAtPlayer()
{

}
void CJumpAttackScript::tick()
{
	CAttackScript::tick();
}

void CJumpAttackScript::begin()
{
	m_pPlayer = 
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)->GetParentObject().at(0);
		
	m_bBeginOn = false;
	CAttackScript::begin();
}

void CJumpAttackScript::BeginOverlap(CCollider3D* _Other)
{
	CAttackScript::BeginOverlap(_Other);

	CMonsterScript* pMonster = _Other->GetOwner()->GetScript<CMonsterScript>();

	CPlayerScript* pPlayer = m_pPlayer->GetScript<CPlayerScript>();
	if (pMonster)
	{
		m_pPlayer->GetScript<CPlayerScript>()->Chanage_AnimDT(2.f);

		
		//m_bBeginOn = true;
		m_bBeginOn = check_pos(_Other->GetOwner());

		if (m_bBeginOn)
		{
			ChanageState(pPlayer->GetFSM(), STATE_TYPE::JUMPEND);
		}
	}
}

void CJumpAttackScript::OnOverlap(CCollider3D* _Other)
{
	//몬스터를 따라서 이동
	if (m_bBeginOn)
	{
		CGameObject* pMonster = _Other->GetOwner()->GetChild().at(0);

		if (_Other->GetOwner()->GetScript<CMonsterScript>()->GetMonsterInfo().fHP <= 0.f)
			return;

		//if(pMonster->GetScript<CMonsterScript>()->GetFSM())
		check_bone_pos(pMonster);
	}
}

void CJumpAttackScript::EndOverlap(CCollider3D* _Other)
{
	CAttackScript::EndOverlap(_Other);

	m_bBeginOn = false;

	m_pPlayer->GetScript<CPlayerScript>()->Chanage_AnimDT(1.f);

	//땅에 도착했을때 끝내는게 아니라 애니메이션 프레임이 끝나면 바꾸게
}


