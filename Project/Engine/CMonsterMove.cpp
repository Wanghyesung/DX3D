#include "pch.h"
#include "CMonsterMove.h"
#include "CNavMesh.h"
#include "CTransform.h"
#include "CRigidbody.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CCollider3D.h"
CMonsterMove::CMonsterMove():
	m_fCheckLen(2000.f),
	m_pTarget(nullptr),
	m_bActive(false)
{
	
}

CMonsterMove::~CMonsterMove()
{
	
}

void CMonsterMove::final_tick()
{
	check_len();

	if (!m_bActive)
		return;

	rotate();

	move();
}

void CMonsterMove::Enter()
{
	Chanage_Anim(GetName());

	const vector<CGameObject*>&  vecObj =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();
	if(vecObj[0] != nullptr)
		m_pTarget = vecObj[0];

	GetOwner()->NavMesh()->SetActive(true);
}

void CMonsterMove::Exit()
{
	GetOwner()->NavMesh()->SetActive(false);
}

void CMonsterMove::check_len()
{
	if (m_pTarget == nullptr)
		return;

	Matrix matTargetWorld = m_pTarget->Collider3D()->GetColliderWorldMat();
	Vec3 vTargetPos = Vec3(matTargetWorld._41, matTargetWorld._42, matTargetWorld._43);

	Matrix matWorld = GetOwner()->Collider3D()->GetColliderWorldMat();
	Vec3 vPos = Vec3(matWorld._41, matWorld._42, matWorld._43);


	float fLen = (vTargetPos - vPos).Length();

	if (fLen <= m_fCheckLen)
		m_bActive = true;
	else
		m_bActive = false;

	//여기서 공격
	
}

void CMonsterMove::rotate()
{
	Vec3 vDir = GetOwner()->NavMesh()->GetTargetPath();
	if (vDir == Vec3::Zero)
		return;

	//z <--> y fbx축 
	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);
	//vFoward.y = 0.f;

	//- = 90 ~ 270도 - 두 벡터의 방향이 둔각을 이룬다.
	float fRadian;
	float fCos = vDir.Dot(vFoward);
	//외적 이용 오른쪽 왼쪽 판별
	Vec3 vCross = vFoward.Cross(vDir);

	if (vCross.y >= 0)
	{
		fRadian = XM_PI + acos(fCos);
	}
	else
	{
		fRadian = XM_PI - acos(fCos);
	}

	//
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	Vec3 vFinalRot = Vec3(-XM_PI / 2.f, fRadian, 0.f);
	Vec3 vLerpRot = Vec3::Lerp(vRot, vFinalRot, DT * 4.f);

	float fDiff = fabs(vFinalRot.y - vRot.y);
	//각도가 크게 변경되었을 때
	if (fDiff >= XM_PI)
		GetOwner()->Transform()->SetRelativeRot(vFinalRot);
	else
		GetOwner()->Transform()->SetRelativeRot(vLerpRot);
}

void CMonsterMove::move()
{
	Vec3 vForward = GetOwner()->Transform()->GetDynamicUp();
	vForward *= -1;
	vForward.y = 0.f;
	GetOwner()->Rigidbody()->AddForce(vForward * 400.f);
}


