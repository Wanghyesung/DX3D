#include "pch.h"
#include "CLandFormScript.h"

CLandFormScript::CLandFormScript():
	CScript(SCRIPT_TYPE::LANDFORMSCRIPT)
{

}

CLandFormScript::~CLandFormScript()
{

}

void CLandFormScript::begin()
{

}

void CLandFormScript::tick()
{

}

void CLandFormScript::BeginOverlap(CCollider3D* _Other)
{

}

void CLandFormScript::OnOverlap(CCollider3D* _Other)
{

}

void CLandFormScript::EndOverlap(CCollider3D* _Other)
{

}

void CLandFormScript::Initialize(const wstring& _strFbxName)
{
	 //InitializeFBX(_strFbxName);
}

void CLandFormScript::SaveToLevelFile(FILE* _File)
{
}

void CLandFormScript::LoadFromLevelFile(FILE* _FILE)
{
}


