#include "pch.h"
#include "CDemonScript.h"
#include "CAttackScript.h"
#include <Engine\CDemonIdle.h>
#include <Engine\CMonsterMove.h>
#include <Engine\CMonsterAttack.h>
#include <Engine\CDemonHit.h>
#include <Engine\CMonsterJump.h>
#include <Engine\CDemonMove.h>
#include <Engine\CDemonWait.h>
#include <Engine\CDemonJump.h>
CDemonScript::CDemonScript():
	CMonsterScript(SCRIPT_TYPE::DEMONSCRIPT),
	m_bJumpTrigger(false)
{

}

CDemonScript::~CDemonScript()
{

}


void CDemonScript::tick()
{
	CMonsterScript::tick();

	if (m_bJumpTrigger)
	{
		Vec3 vFront = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
		Vec3 vForce = -vFront * m_vJumpForce;

		GetOwner()->PxRigidbody()->SetVelocity(vForce);
	}
}

void CDemonScript::BeginOverlap(CCollider3D* _Other)
{
	CAttackScript* pAttack = _Other->GetOwner()->GetScript<CAttackScript>();

	if (pAttack)
		int a = 10;

}

void CDemonScript::OnOverlap(CCollider3D* _Other)
{
	CMonsterScript::OnOverlap(_Other);
}

void CDemonScript::EndOverlap(CCollider3D* _Other)
{

}

void CDemonScript::begin()
{
	CMonsterScript::begin();
	//이벤트 설정
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
	AddMonsterState(MONSTER_STATE_TYPE::JUMP, pJump, L"Jump", 1686, 1920);

	CDemonWait* pWait = new CDemonWait();
	AddMonsterState(MONSTER_STATE_TYPE::WAIT, pWait, L"Wait", 1686, 1686);

	CDemonHit* pHit = new CDemonHit();
	AddMonsterState(MONSTER_STATE_TYPE::HIT, pHit, L"Hit", 527, 650);

	AddAnimFrame(L"Attack0", 651, 792);
	AddAnimFrame(L"Attack1", 977, 1114);
	AddAnimFrame(L"Attack2", 1115, 1266);
	AddAnimFrame(L"Attack3", 1267, 1427);

	set_attack();

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack3", std::bind(&CDemonScript::jump_start, this), 1305);

	GetOwner()->GetChild().at(0)->Animator3D()->
		AddEvent(L"Attack3", std::bind(&CDemonScript::jump_end, this), 1321);
}

void CDemonScript::jump_start()
{
	m_bJumpTrigger = true;

	m_vJumpForce = Vec3(0.f, 1000.f, 0.f);
}

void CDemonScript::jump_end()
{
	m_bJumpTrigger = false;

	m_vJumpForce = Vec3::Zero;
}

void CDemonScript::set_attack()
{
	AddMonsterAttack(0, 0.f, 0.f, 0.f, 695, 700, Vec3(400.f, 200.f, 400.f), 400.f, Vec3::Zero);
	AddMonsterAttack(1, 0.f, 0.f, 0.f, 1020, 1023, Vec3(400.f, 200.f, 400.f), 400.f, Vec3::Zero);
	AddMonsterAttack(2, 0.f, 0.f, 0.f, 1175, 1177, Vec3(400.f, 200.f, 400.f), 400.f, Vec3::Zero);
	AddMonsterAttack(3, 0.f, 0.f, 0.f, 1323, 1326, Vec3(300.f, 200.f, 300.f), 440.f, Vec3::Zero);
}