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

	//z <--> y fbx�� 
	Vec3 vFoward = Vec3(0.f, 0.f, -1.f);
	//vFoward.y = 0.f;

	//- = 90 ~ 270�� - �� ������ ������ �а��� �̷��.
	float fRadian;
	float fCos = vDir.Dot(vFoward);
	//���� �̿� ������ ���� �Ǻ�
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


