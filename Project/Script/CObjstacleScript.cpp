#include "pch.h"
#include "CObjstacleScript.h"

CObjstacleScript::CObjstacleScript():
	CScript(SCRIPT_TYPE::OBJSTACLESCRIPT)
{
}

CObjstacleScript::~CObjstacleScript()
{
}

void CObjstacleScript::begin()
{
}

void CObjstacleScript::tick()
{
}

void CObjstacleScript::BeginOverlap(CCollider3D* _Other)
{
}

void CObjstacleScript::OnOverlap(CCollider3D* _Other)
{
}

void CObjstacleScript::EndOverlap(CCollider3D* _Other)
{
}

void CObjstacleScript::Initialize(const wstring& _strFbxName)
{
}

void CObjstacleScript::SaveToLevelFile(FILE* _File)
{
}

void CObjstacleScript::LoadFromLevelFile(FILE* _FILE)
{
}

