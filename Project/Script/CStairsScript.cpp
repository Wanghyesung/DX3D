#include "pch.h"
#include "CStairsScript.h"
#include "CPlayerScript.h"
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
}

void CStairsScript::EndOverlap(CCollider3D* _Other)
{
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

