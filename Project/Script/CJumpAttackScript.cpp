#include "pch.h"
#include "CJumpAttackScript.h"
#include "CMonsterScript.h"
void CJumpAttackScript::tick()
{
	CAttackScript::tick();
}

void CJumpAttackScript::begin()
{
	CAttackScript::begin();
}

void CJumpAttackScript::BeginOverlap(CCollider3D* _Other)
{
	CAttackScript::BeginOverlap(_Other);

}

void CJumpAttackScript::OnOverlap(CCollider3D* _Other)
{
}

void CJumpAttackScript::EndOverlap(CCollider3D* _Other)
{
	CAttackScript::EndOverlap(_Other);

}

CJumpAttackScript::CJumpAttackScript():
	CAttackScript(SCRIPT_TYPE::JUMPATTACKSCRIPT)
{
}

CJumpAttackScript::~CJumpAttackScript()
{
}
