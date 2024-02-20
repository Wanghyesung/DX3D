#include "pch.h"
#include "CBossStageScript.h"
#include "CPlayerScript.h"
#include <Engine\CTransform.h>
#include <Engine\CRigidbody.h>
#include <Engine\CCollider3D.h>

void CBossStageScript::begin()
{
	//�浹ü ����
}

void CBossStageScript::tick()
{

}

void CBossStageScript::BeginOverlap(CCollider3D* _Other)
{
	int a = 10;
}

void CBossStageScript::OnOverlap(CCollider3D* _Other)
{
	CGameObject* pObj = _Other->GetOwner();
	CPlayerScript* pScript = pObj->GetScript<CPlayerScript>();

	if (pScript)
	{
		int a = 10;
	}
}

void CBossStageScript::EndOverlap(CCollider3D* _Other)
{

}


CBossStageScript::CBossStageScript():
	CScript(SCRIPT_TYPE::BOSSSTAGESCRIPT)
{
	

}

CBossStageScript::~CBossStageScript()
{

}
