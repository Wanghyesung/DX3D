#include "pch.h"
#include "CDemonScript.h"
#include "CAttackScript.h"
#include "CObjstacleScript.h"
#include "CMonsterAttackScript.h"
#include "CJumpAttackScript.h"

#include <Engine\CMonsterFSM.h>
#include <Engine\CDemonIdle.h>
#include <Engine\CMonsterMove.h>
#include <Engine\CMonsterAttack.h>
#include <Engine\CDemonHit.h>
#include <Engine\CMonsterJump.h>
#include <Engine\CDemonMove.h>
#include <Engine\CDemonWait.h>
#include <Engine\CDemonJump.h>
#include <Engine\CDemonJumpAttack.h>
#include <Engine\CMonsterDead.h>
#include <Engine\CFontMgr.h>
CDemonScript::CDemonScript():
	CMonsterScript(SCRIPT_TYPE::DEMONSCRIPT)
{

}

CDemonScript::~CDemonScript()
{

}


void CDemonScript::tick()
{
	
	//const Matrix& mat = GetOwner()->Collider3D()->GetColliderWorldMat();
	//DrawDebugCylinder(mat, Vec4(0.f, 0.f, 1.f, 1.f), 0.f, false);
	//
	CMonsterScript::tick();

	//
}

void CDemonScript::BeginOverlap(CCollider3D* _Other)
{
	//CGameObject* pObj = _Other->GetOwner();
	//CObjstacleScript* pObstacle = pObj->GetScript<CObjstacleScript>();
	//
	//if (pObstacle)
	//{
	//	MONSTER_STATE_TYPE eState = m_pFSM->GetCurStateType();
	//	if (eState == MONSTER_STATE_TYPE::JUMP)
	//	{
	//		CMonsterState* pState = m_pFSM->FindState(eState);
	//		CDemonJump* pJump = dynamic_cast<CDemonJump*>(pState);
	//
	//		pJump->SetAttackTrigger();
	//	}
	//}
}

void CDemonScript::OnOverlap(CCollider3D* _Other)
{
	//CMonsterScript::OnOverlap(_Other);

	CGameObject* pObj = _Other->GetOwner();

	CAttackScript* pAttack = pObj->GetScript<CAttackScript>();
	if (pAttack)
	{
		Vec3 vAttackPos = pObj->PxRigidbody()->GetPxPosition();
		Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

		//데미지 위에서 맞게
		float fYLen = vAttackPos.y - vPos.y;

		bool bOn = pAttack->IsAttackOn(GetOwner()->GetID());
		if (bOn)
		{
			//공중에서 맞을 때 4배 데미지
			if (fYLen >= 300.f)
			{
				CMonsterHit* pHit = dynamic_cast<CMonsterHit*>(m_pFSM->FindState(MONSTER_STATE_TYPE::HIT));
				pHit->SetHitInfo(m_tHitInfo);
				m_tMonsterInfo.fHP -= (m_tHitInfo.fDamage * 4);

				if (m_pFSM->GetCurStateType() == MONSTER_STATE_TYPE::HIT)
					return;

				ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::HIT);
			}

			//일반 데미지
			else
			{
				m_tMonsterInfo.fHP -= m_tHitInfo.fDamage;
			}

			if (m_tMonsterInfo.fHP <= 0.f)
			{
				if (m_bDead)
					return;

				ChanageMonsterState(m_pFSM, MONSTER_STATE_TYPE::DEAD);
				m_bDead = true;
			}

			m_pHp->UpdateGage(m_tMonsterInfo.fMaxHP, m_tMonsterInfo.fHP);
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
	m_tMonsterInfo.fHP = 1000.f;
	m_tMonsterInfo.fMaxHP = 1000.f;

	CMonsterScript::Initialize(_strFbxName);

	CDemonIdle* pIdle = new CDemonIdle();
	AddMonsterState(MONSTER_STATE_TYPE::IDLE, pIdle, L"Idle", 0, 119);

	CDemonMove* pMove = new CDemonMove();
	AddMonsterState(MONSTER_STATE_TYPE::WALK, pMove, L"Walk", 121, 180);

	CMonsterAttack* pAttack = new CMonsterAttack();
	AddMonsterState(MONSTER_STATE_TYPE::ATTACK, pAttack, L"Attack");

	CDemonJump* pJump = new CDemonJump();
	AddMonsterState(MONSTER_STATE_TYPE::JUMP, pJump, L"Jump", 1686, 1758);//1920

	CDemonJumpAttack* pJumpAttack = new CDemonJumpAttack();
	AddMonsterState(MONSTER_STATE_TYPE::JUMP_ATTACK, pJumpAttack, L"Jump_Attack", 1758, 1921);

	CDemonWait* pWait = new CDemonWait();
	AddMonsterState(MONSTER_STATE_TYPE::WAIT, pWait, L"Wait", 1686, 1686);

	CDemonHit* pHit = new CDemonHit();
	AddMonsterState(MONSTER_STATE_TYPE::HIT, pHit, L"Hit", 527, 650);

	CMonsterDead* pDead = new CMonsterDead();
	AddMonsterState(MONSTER_STATE_TYPE::DEAD, pDead, L"Dead", 3110, 3274);

	AddAnimFrame(L"Walk_Back", 182, 241);
	
	AddAnimFrame(L"Attack0", 651, 792);
	AddAnimFrame(L"Attack1", 977, 1114);
	AddAnimFrame(L"Attack2", 1115, 1266);
	AddAnimFrame(L"Attack3", 1267, 1427);

	set_attack();

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack3", std::bind(&CDemonScript::jump_start, this), 1305);

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack3", std::bind(&CDemonScript::jump_end, this), 1319);

	m_pFSM->SetState(MONSTER_STATE_TYPE::IDLE);


	//init_hp();

	CGameObject* pWeapon = InitializeFBX(L"TaurusDemon_Axe");
	pWeapon->AddComponent(new CTransform());
	pWeapon->AddComponent(new CEquip());

	pWeapon->Transform()->SetRelativeScale(Vec3(4.6f, 4.6f, 4.6f));

	//-500 
	float fRadian = XM_PI / 180.f;
	pWeapon->Transform()->SetRelativeRot(-90.f * fRadian, 180.f * fRadian, -15.f * fRadian);

	pWeapon->Transform()->SetAbsolute(true);
	//-90 270 -9

	SpawnGameObject(pWeapon, Vec3(-330.f, -50.f, 360.f), (int)LAYER_TYPE::Monster);
	pWeapon->Equip()->SetChar(GetOwner());
	pWeapon->Equip()->SetIndex(38);//손 뼈 인덱스
	//pWeapon->Equip()->SetFixedPos()
}

void CDemonScript::jump_start()
{
	m_vJumpForce = Vec3(0.f, 250.f, 0.f);

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
	

	tAttackInfo attackjump = {};
	//attackjump.iStartFrame = 954;
	//attackjump.iEndFrame = 960;
	attackjump.vAttackScale = Vec3(1000.f, 600.f, 1000.f);
	attackjump.tAttackValue.iMaxCount = 1;
	attackjump.tAttackValue.fAttRcnt = 200.f;
	attackjump.tAttackValue.fDamage = 40.f;
	attackjump.tAttackValue.fAddForceTime = 0.7f;
	attackjump.tAttackValue.bDown = false;

	CMonsterAttackScript* pJumpAttack = new CMonsterAttackScript();
	pJumpAttack->SetAttackValue(attackjump.tAttackValue);
	CGameObject* pAttackObj = new CGameObject();
	CCollider3D* pCollider = new CCollider3D();
	pCollider->SetOffsetScale(attackjump.vAttackScale);
	pAttackObj->AddComponent(pCollider);
	pAttackObj->AddComponent(new CTransform);
	pAttackObj->AddComponent(pJumpAttack);

	CPxRigidbody* pRigid = new CPxRigidbody();
	pAttackObj->AddComponent(pRigid);
	pRigid->init(Vec3(-2000.f, -2000.f, -2000.f), attackjump.vAttackScale, (int)LAYER_TYPE::MonsterAttack, pAttackObj);
	pRigid->SetGround(true, false); //땅상태 , 중력 안받음 
	pRigid->SetPass(true); // 충돌해도 통과되게

	CMonsterState* pState = m_pFSM->FindState(MONSTER_STATE_TYPE::JUMP_ATTACK);
	CDemonJumpAttack* pJumpingState = dynamic_cast<CDemonJumpAttack*>(pState);
	pJumpingState->SetAttackObj(pAttackObj);
}

void CDemonScript::init_hp()
{
	CGameObject* pHP = new CGameObject();
	m_pHp = new CMonsterHPScript();
	m_pHp->Initialize(L"texture\\GameTexture\\Monster", Vec3(950.f, 35.f, 1.f), 
		GetOwner()->GetName(),true, L"소머리 데몬");

	m_pHp->SetMonster(GetOwner());

	pHP->AddComponent(m_pHp);
	pHP->AddComponent(new CTransform());
	SpawnGameObject(pHP, Vec3::Zero, (int)LAYER_TYPE::Default);//2차원 공간
}
