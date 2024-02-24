#include "pch.h"
#include "CLandScpaeScript.h"

#include "CPlayerScript.h"
#include <Engine\CGameObject.h>
#include <Engine\CPxRigidbody.h>
#include <Engine\CCollider3D.h>
void CLandScpaeScript::begin()
{

}

void CLandScpaeScript::tick()
{

}

void CLandScpaeScript::BeginOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();

	CPlayerScript* pScript = pObj->GetScript<CPlayerScript>();
	if (pScript)
	{
		int a = 10;
	}
}

void CLandScpaeScript::OnOverlap(CCollider3D* _Other)
{
	//CGameObject* pObj = _Other->GetOwner();
	//
	//CPlayerScript* pScript = pObj->GetScript<CPlayerScript>();
	//if (pScript)
	//{
	//	pObj->PxRigidbody()->SetGround(true);
	//}
}

void CLandScpaeScript::EndOverlap(CCollider3D* _Other)
{
	
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
