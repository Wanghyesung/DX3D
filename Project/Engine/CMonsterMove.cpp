#include "pch.h"
#include "CMonsterMove.h"
#include "CNavMesh.h"
#include "CTransform.h"
#include "CRigidbody.h"
CMonsterMove::CMonsterMove() :
	m_pTarget(nullptr)
{
	
}

CMonsterMove::~CMonsterMove()
{

}

void CMonsterMove::final_tick()
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

	GetOwner()->Rigidbody()->AddForce(vDir* 400.f);
	if (vCross.y >= 0)
	{
		fRadian = XM_PI + acos(fCos);
	}
	else
	{
		fRadian = XM_PI - acos(fCos);
	}
	GetOwner()->Transform()->SetRelativeRot(Vec3(-XM_PI / 2.f, fRadian, 0.f));
}

void CMonsterMove::Enter()
{
	//GetOwner()->NavMesh()->Initialize();
}

void CMonsterMove::Exit()
{

}


