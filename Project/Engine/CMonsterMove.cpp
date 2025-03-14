#include "pch.h"
#include "CMonsterMove.h"
#include "CNavMesh.h"
#include "CRDNavMeshField.h"
#include "CTransform.h"
#include "CRigidbody.h"
#include "CPxRigidbody.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CCollider3D.h"
#include "CNavMeshMgr.h"
#include "CRDNavMeshField.h"

CMonsterMove::CMonsterMove() :
	m_fCheckLen(1000.f),
	m_fAttackLen(170.f),
	m_pTarget(nullptr),
	m_fStopLen(200.f)
{
	
}


CMonsterMove::~CMonsterMove()
{
	
}

void CMonsterMove::final_tick()
{
	if (m_pTarget == nullptr)
		return;

	if (check_len())
		return;
	else
	{
		if (!rotate())
			return;

		move();
	}
}

void CMonsterMove::Enter()
{
	Chanage_Anim(GetName());

	const vector<CGameObject*>&  vecObj =
		CLevelMgr::GetInst()->GetCurLevel()->GetLayer((UINT)LAYER_TYPE::Player)->GetParentObject();
	if(vecObj[0] != nullptr)
		m_pTarget = vecObj[0];

	m_fCheckLen =
		GetOwner()->RDNavMeshField()->GetSearchRange();

	GetOwner()->RDNavMeshField()->SetActive(true);
	CNavMeshMgr::GetInst()->AddNavMeshField(GetOwner()->RDNavMeshField()); //다른 스레드에서 자동으로 범위 체크

}

void CMonsterMove::Exit()
{
	GetOwner()->RDNavMeshField()->SetActive(false);
	CNavMeshMgr::GetInst()->DeleteNavMeshField(GetOwner()->GetID());

}

bool CMonsterMove::check_len()
{
	Vec3 vTargetPos = m_pTarget->Collider3D()->GetWorldPos();

	Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();

	float fLen = (vTargetPos - vPos).Length();


	if (fLen >= m_fCheckLen)
	{
		//여기서 플레어 방향까지 회전
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
		return true;
	}
	else if (fLen <= m_fAttackLen)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::ATTACK);
		return true;
	}

	return false;
}

bool CMonsterMove::rotate()
{
	Vec3 vDir = GetOwner()->RDNavMeshField()->GetFindPath();
	
	if (vDir == Vec3::Zero)
		return false;
	
	//z <--> y fbx축 
	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);
	//vFoward.y = 0.f;
	
	//- = 90 ~ 270도 - 두 벡터의 방향이 둔각을 이룬다.
	float fRadian;
	float fCos = vDir.Dot(vFoward);
	//외적 이용 오른쪽 왼쪽 판별
	Vec3 vCross = vFoward.Cross(vDir);
	
	if (vCross.y >= 0)
		fRadian = XM_PI + acos(fCos);
	else
		fRadian = XM_PI - acos(fCos);
	
	//
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	Vec3 vFinalRot = Vec3(-XM_PI / 2.f, fRadian, 0.f);
	Vec3 vLerpRot = Vec3::Lerp(vRot, vFinalRot, DT * 4.f);
	Vec3 vPxRot = {};
	
	float fDiff = fabs(vFinalRot.y - vRot.y);
	//각도가 크게 변경되었을 때
	if (fDiff >= XM_PI)
	{
		GetOwner()->Transform()->SetRelativeRot(vFinalRot);
		vPxRot = vFinalRot;
	}
		
	else
	{
		GetOwner()->Transform()->SetRelativeRot(vLerpRot);
		vFinalRot = vLerpRot;
	}
		
	
	PxQuat yRotation(vFinalRot.y, PxVec3(0.0f, 1.0f, 0.0f));
	GetOwner()->PxRigidbody()->SetPxRotate(yRotation);

	return true;
}


void CMonsterMove::move()
{
	Vec3 vForward = GetOwner()->Transform()->GetDynamicUp();
	vForward *= -1;
	vForward.y = 0.f;
	GetOwner()->PxRigidbody()->AddVelocity(vForward * 400.f);
}

void CMonsterMove::lookat_player()
{
	Vec3 vTargetPos = m_pTarget->Collider3D()->GetWorldPos();
	Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();

	Vec3 vDir = (vTargetPos - vPos).Normalize();

	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);

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

	Vec3 vFinalRot = Vec3(-XM_PI / 2.f, fRadian, 0.f);
	GetOwner()->Transform()->SetRelativeRot(vFinalRot);
}


