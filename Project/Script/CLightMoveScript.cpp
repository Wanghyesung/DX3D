#include "pch.h"
#include "CLightMoveScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CRenderMgr.h>
CLightMoveScript::CLightMoveScript():
	CScript(SCRIPT_TYPE::LIGHTMOVESCRIPT),
	m_pTarget(nullptr)
{
}

CLightMoveScript::~CLightMoveScript()
{
}


void CLightMoveScript::tick()
{
	if (!m_pTarget)
		return;

	if (m_pTarget->IsDead())
		m_pTarget = nullptr;


	Light3DMove();
}

void CLightMoveScript::begin()
{
	m_pTarget = CLevelMgr::GetInst()->GetCurLevel()->GetLayer((int)LAYER_TYPE::Player)
		->GetParentObject().at(0);
}


void CLightMoveScript::Light3DMove()
{
	//내 초기 위치를 고정하고 main카메라 위치에 더하기
	static Vec3 vLightPos; 
	if (vLightPos == Vec3::Zero)
		vLightPos = GetOwner()->Transform()->GetRelativePos();

	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	Vec3 vCamPos = pMainCam->GetOwner()->Transform()->GetRelativePos();
	

	GetOwner()->Transform()->SetRelativePos(vLightPos + vCamPos);
}


void CLightMoveScript::move_target()
{
}

void CLightMoveScript::move_editor()
{
	
}
