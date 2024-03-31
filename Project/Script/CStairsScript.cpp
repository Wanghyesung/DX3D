#include "pch.h"
#include "CStairsScript.h"
#include "CPlayerScript.h"
#include <Engine\CFSM.h>
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
	}
}

void CStairsScript::OnOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPxRigidbody* pRigid = pObj->PxRigidbody();
	if (pRigid)
	{
		CPlayerScript* pPlayer = pObj->GetScript<CPlayerScript>();

		if (pPlayer)
		{
			if (pPlayer->GetFSM()->GetCurStateType() == STATE_TYPE::JUMP)
				return;
		}

		pObj->PxRigidbody()->SetGround(true);
	}
}

void CStairsScript::EndOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPxRigidbody* pRigid = pObj->PxRigidbody();
	if (pRigid)
	{
		pObj->PxRigidbody()->SetGround(false);
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

