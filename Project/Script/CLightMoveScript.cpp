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

	m_vLightPos = GetOwner()->Transform()->GetRelativePos();
}


void CLightMoveScript::Light3DMove()
{
	//�� �ʱ� ��ġ�� �����ϰ� mainī�޶� ��ġ�� ���ϱ�
	
	Vec3 vPlayerPos = m_pTarget->Transform()->GetRelativePos();
	vPlayerPos.y = 0;

	GetOwner()->Transform()->SetRelativePos(m_vLightPos + vPlayerPos);
}


void CLightMoveScript::move_target()
{
}

void CLightMoveScript::move_editor()
{
	
}
