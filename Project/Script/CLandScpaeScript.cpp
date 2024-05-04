#include "pch.h"
#include "CLandScpaeScript.h"

#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include <Engine\CGameObject.h>
#include <Engine\CPxRigidbody.h>
#include <Engine\CCollider3D.h>
#include <Engine\CFSM.h>
#include <Engine\CMonsterFSM.h>

void CLandScpaeScript::begin()
{

}

void CLandScpaeScript::tick()
{

}

void CLandScpaeScript::BeginOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPxRigidbody* pRigid = pObj->PxRigidbody();
	if (pRigid)
	{
		CPlayerScript* pPlayer = pObj->GetScript<CPlayerScript>();
		//CMonsterScript* pMonster = pObj->GetScript<CMonsterScript>();
		if (pPlayer)
		{
			if (pPlayer->GetFSM()->GetCurStateType() == STATE_TYPE::JUMP)
				return;
		}
		//else if (pMonster)
		//{
		//	if (pMonster->GetFSM()->GetCurStateType() == MONSTER_STATE_TYPE::JUMP)
		//		return;
		//}

		pObj->PxRigidbody()->SetGround(true);

		//중력 크게 받지 않도록
		pObj->PxRigidbody()->SetAddGravity(false);
	}
}

void CLandScpaeScript::OnOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPxRigidbody* pRigid = pObj->PxRigidbody();
	if (pRigid)
	{
		CPlayerScript* pPlayer = pObj->GetScript<CPlayerScript>();
		//CMonsterScript* pMonster = pObj->GetScript<CMonsterScript>();
		if (pPlayer)
		{
			if (pPlayer->GetFSM()->GetCurStateType() == STATE_TYPE::JUMP)
				return;
		}
		//else if (pMonster)
		//{
		//	if (pMonster->GetFSM()->GetCurStateType() == MONSTER_STATE_TYPE::JUMP)
		//		return;
		//}
	}
}

void CLandScpaeScript::EndOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPxRigidbody* pRigid = pObj->PxRigidbody();
	if (pRigid)
	{
		pObj->PxRigidbody()->SetGround(false);
	}
}

void CLandScpaeScript::Initialize(const wstring& _strFbxName)
{

}

void CLandScpaeScript::SaveToLevelFile(FILE* _File)
{
}

void CLandScpaeScript::LoadFromLevelFile(FILE* _FILE)
{
}


CLandScpaeScript::CLandScpaeScript():
	CScript(SCRIPT_TYPE::LANDSCAPESCRIPT)
{
	
}

CLandScpaeScript::~CLandScpaeScript()
{
}
