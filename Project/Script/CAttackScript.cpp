#include "pch.h"
#include "CAttackScript.h"
#include <Engine\CAttackState.h>

CAttackScript::CAttackScript() :
	CScript((UINT)SCRIPT_TYPE::ATTACKSCRIPT)
{
}

CAttackScript::~CAttackScript()
{

}

void CAttackScript::add_monster(UINT _ID)
{
	//첫 공격
	m_vecAbleAttack[_ID] = 1;
}

UINT CAttackScript::find_monster(UINT _ID)
{
	if (m_vecAbleAttack.size() < _ID)
	{
		m_vecAbleAttack.resize(_ID + 1);
	}
	return m_vecAbleAttack[_ID]; 
	//0 없음 1있음
}

void CAttackScript::tick()
{

}

void CAttackScript::begin()
{
	m_vecAbleAttack.clear();
}

void CAttackScript::BeginOverlap(CCollider3D* _Other)
{
	int a = 10;
	//find_monster()
}

void CAttackScript::OnOverlap(CCollider3D* _Other)
{

}

void CAttackScript::EndOverlap(CCollider3D* _Other)
{
	int a = 10;
}
