#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>



CCameraMoveScript::CCameraMoveScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_fCamSpeed(500.f)
	, m_pTarget(nullptr)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		Camera2DMove();
	else
		Camera3DMove();
}

void CCameraMoveScript::Camera2DMove()
{
	// 키 입력에 따른 카메라 이동
	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fCamSpeed;
	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * fSpeed;
	}


	if (KEY_PRESSED(KEY::_1))
	{
		float fScale = Camera()->GetScale();
		fScale += DT * 1.f;
		Camera()->SetScale(fScale);
	}

	if (KEY_PRESSED(KEY::_2))
	{
		float fScale = Camera()->GetScale();
		fScale -= DT * 1.f;
		Camera()->SetScale(fScale);
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::Camera3DMove()
{
	if (!m_pTarget)
	{
		move_editor();
	}

	else
	{
		move_target();
	}
}

void CCameraMoveScript::move_target()
{
	//Vec3 vRot = Transform()->GetRelativeRot();
	//Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
	//vRot.y += (DT/2.f * vMouseDir.x);
	//
	//Vec3 vNewRot = {};
	//vNewRot.y = vRot.y;
	//vNewRot.x = -XM_PI / 2.f;
	//
	////player
	//m_pTarget->Transform()->SetRelativeRot(vNewRot);
	Vec3 vTargetFoword = m_pTarget->Transform()->GetDynamicUp();

	//foward -> z축 y축 반전
	//up->y축 z축 반전 - 1
	//forardvector -> upvector

	//Vec3 vNewTargetFoword = {};
	//vNewTargetFoword.x = vTargetFoword.x;
	//vNewTargetFoword.y = -vTargetFoword.z;
	//vNewTargetFoword.z = vTargetFoword.y;

	Vec3 vTargetRot = m_pTarget->Transform()->GetRelativeRot();

	//camera
	Vec3 vRot = Transform()->GetRelativeRot();
	vRot.x = XM_PI / 10.f;
	vRot.y = vTargetRot.y;
	//vTargetFoword *= -1;
	float fMaxDistance = 1000.f;
	vTargetFoword *= fMaxDistance;

	Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
	Vec3 vFinalPos = vTargetPos + vTargetFoword;
	vFinalPos.y += 500.f;

	Transform()->SetRelativeRot(vRot);
	//GlobalData.CameraRot = vRot;
	Transform()->SetRelativePos(vFinalPos);
}

void CCameraMoveScript::move_editor()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos += DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos -= DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos -= DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += DT * vRight * fSpeed;
	}


	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * 5.f;
		vRot.x -= DT * vMouseDir.y * 5.f;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}
