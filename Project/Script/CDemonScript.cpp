#include "pch.h"
#include "CDemonScript.h"
#include "CAttackScript.h"
#include "CObjstacleScript.h"
#include <Engine\CMonsterFSM.h>
#include <Engine\CDemonIdle.h>
#include <Engine\CMonsterMove.h>
#include <Engine\CMonsterAttack.h>
#include <Engine\CDemonHit.h>
#include <Engine\CMonsterJump.h>
#include <Engine\CDemonMove.h>
#include <Engine\CDemonWait.h>
#include <Engine\CDemonJump.h>

CDemonScript::CDemonScript():
	CMonsterScript(SCRIPT_TYPE::DEMONSCRIPT)
{

}

CDemonScript::~CDemonScript()
{

}


void CDemonScript::tick()
{
	int a = 10;
	//const Matrix& mat = GetOwner()->Collider3D()->GetColliderWorldMat();
	//DrawDebugCylinder(mat, Vec4(0.f, 0.f, 1.f, 1.f), 0.f, false);
	//
	CMonsterScript::tick();
	//
}

void CDemonScript::BeginOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();
	CObjstacleScript* pObstacle = pObj->GetScript<CObjstacleScript>();
	
	if (pObstacle)
	{
		MONSTER_STATE_TYPE eState = m_pFSM->GetCurStateType();
		if (eState == MONSTER_STATE_TYPE::JUMP)
		{
			CMonsterState* pState = m_pFSM->FindState(eState);
			CDemonJump* pJump = dynamic_cast<CDemonJump*>(pState);

			pJump->SetAttackTrigger();
		}
	}
}

void CDemonScript::OnOverlap(CCollider3D* _Other)
{
	//CMonsterScript::OnOverlap(_Other);

	CGameObject* pObj = _Other->GetOwner();

	CAttackScript* pAttack = pObj->GetScript<CAttackScript>();
	if (pAttack)
	{
		Vec3 vPlayerPos = pObj->PxRigidbody()->GetPxPosition();
		Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

		float fYLen = vPos.y - vPlayerPos.y;

		if (fYLen >= 200.f)
		{
			bool bOn = pAttack->IsAttackOn(GetOwner()->GetID());
			if (bOn)
			{
				CMonsterHit* pHit = dynamic_cast<CMonsterHit*>(m_pFSM->FindState(MONSTER_STATE_TYPE::HIT));
				pHit->SetHitInfo(m_tHitInfo);


				if (m_pFSM->GetCurStateType() == MONSTER_STATE_TYPE::HIT)
					return;

				ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::HIT);
			}
		}


	}
}

void CDemonScript::EndOverlap(CCollider3D* _Other)
{

}

void CDemonScript::begin()
{
	CMonsterScript::begin();
	//이벤트 설정

	PxQuat yRotation(XM_PI /3.f, PxVec3(0.0f, 1.0f, 0.0f));
	GetOwner()->PxRigidbody()->SetPxRotate(yRotation);

}

void CDemonScript::Initialize(const wstring& _strFbxName)
{
	CMonsterScript::Initialize(_strFbxName);

	CDemonIdle* pIdle = new CDemonIdle();
	AddMonsterState(MONSTER_STATE_TYPE::IDLE, pIdle, L"Idle", 0, 119);

	CDemonMove* pMove = new CDemonMove();
	AddMonsterState(MONSTER_STATE_TYPE::WALK, pMove, L"Walk", 121, 180);

	CMonsterAttack* pAttack = new CMonsterAttack();
	AddMonsterState(MONSTER_STATE_TYPE::ATTACK, pAttack, L"Attack");

	CDemonJump* pJump = new CDemonJump();
	AddMonsterState(MONSTER_STATE_TYPE::JUMP, pJump, L"Jump", 1686, 1758);//1920


	CDemonWait* pWait = new CDemonWait();
	AddMonsterState(MONSTER_STATE_TYPE::WAIT, pWait, L"Wait", 1686, 1686);

	CDemonHit* pHit = new CDemonHit();
	AddMonsterState(MONSTER_STATE_TYPE::HIT, pHit, L"Hit", 527, 650);

	AddAnimFrame(L"Walk_Back", 182, 241);
	AddAnimFrame(L"Jump_Attack", 1759, 1921);

	AddAnimFrame(L"Attack0", 651, 792);
	AddAnimFrame(L"Attack1", 977, 1114);
	AddAnimFrame(L"Attack2", 1115, 1266);
	AddAnimFrame(L"Attack3", 1267, 1427);

	set_attack();

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack3", std::bind(&CDemonScript::jump_start, this), 1305);

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack3", std::bind(&CDemonScript::jump_end, this), 1321);

	m_pFSM->SetState(MONSTER_STATE_TYPE::IDLE);
}

void CDemonScript::jump_start()
{
	m_vJumpForce = Vec3(0.f, 200.f, 0.f);

	tAttackInfo& tInfo = m_pFSM->GetAttackInfo(3);

	Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	vFront.y = 0.f;
	
	Vec3 vFrontForce = -vFront * tInfo.fForce;
	Vec3 vFinalPos = vFrontForce + m_vJumpForce;
	
	tInfo.vForce = vFinalPos;
	tInfo.fMoveTime = 1.f;
}

void CDemonScript::jump_end()
{
	m_vJumpForce = Vec3::Zero;

	tAttackInfo& tInfo = m_pFSM->GetAttackInfo(3);
	tInfo.vForce = m_vJumpForce;
	tInfo.fMoveTime = 0.f;
	//GetOwner()->PxRigidbody()->SetAddGravity(true);
}

void CDemonScript::set_attack()
{
	AddMonsterAttack(0,0.f, 0.f, 0.f, 695, 700, Vec3(400.f, 200.f, 400.f), 400.f, Vec3::Zero);
	AddMonsterAttack(1,0.f, 0.f, 0.f, 1020, 1023, Vec3(400.f, 200.f, 400.f), 400.f, Vec3::Zero);
	AddMonsterAttack(2,0.f, 0.f, 0.f, 1175, 1177, Vec3(400.f, 200.f, 400.f), 400.f, Vec3::Zero);
	AddMonsterAttack(3, 800.f, 0.f, 0.f, 1323, 1326, Vec3(300.f, 200.f, 300.f), 440.f, Vec3::Zero);
}