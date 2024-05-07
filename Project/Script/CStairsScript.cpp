#include "pch.h"
#include "CStairsScript.h"
#include "CPlayerScript.h"
#include "CDemonScript.h"
#include <Engine\CFSM.h>
#include <Engine\CMonsterFSM.h>
CStairsScript::CStairsScript():
	CScript(SCRIPT_TYPE::STAIRSSCRIPT)
{
}

CStairsScript::~CStairsScript()
{
}


void CStairsScript::begin()
{
}

void CStairsScript::tick()
{
}

void CStairsScript::BeginOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPlayerScript* pScript = pObj->GetScript<CPlayerScript>();
	if (pScript)
	{
		pObj->PxRigidbody()->SetGround(true);
		pObj->PxRigidbody()->SetAddGravity(false);
	}
}

void CStairsScript::OnOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPxRigidbody* pRigid = pObj->PxRigidbody();
	if (pRigid)
	{
		CPlayerScript* pPlayer = pObj->GetScript<CPlayerScript>();
		//CDemonScript* pMonster = pObj->GetScript<CDemonScript>();
		if (pPlayer)
		{
			if (pPlayer->GetFSM()->GetCurStateType() == STATE_TYPE::JUMP)
				return;
		}
		//else if (pMonster)
		//{
		//	if (pMonster->GetFSM()->GetCurStateType() == MONSTER_STATE_TYPE::JUMP_ATTACK)
		//		return;
		//}

		pObj->PxRigidbody()->SetAddGravity(true);
		pObj->PxRigidbody()->SetGround(true);
	}
}

void CStairsScript::EndOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();
	
	CPxRigidbody* pRigid = pObj->PxRigidbody();
	if (pRigid)
	{
		pObj->PxRigidbody()->SetAddGravity(true);
		//pObj->PxRigidbody()->SetGround(false);
	}
}

void CStairsScript::Initialize(const wstring& _strFbxName)
{
}

void CStairsScript::SaveToLevelFile(FILE* _File)
{
}

void CStairsScript::LoadFromLevelFile(FILE* _FILE)
{
}

