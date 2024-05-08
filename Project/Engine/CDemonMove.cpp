#include "pch.h"
#include "CDemonMove.h"
#include "CNavMesh.h"
#include "CPxRigidbody.h"
#include "CRDNavMeshField.h"
#include "CCollider3D.h"
#include "CTransform.h"
CDemonMove::CDemonMove():
	m_fMaxRadian(XM_PI / 2.1f)
	, m_fMaxDegree(15.f)
{
	SetCheckLen(3100.f);
}

CDemonMove::~CDemonMove()
{

}


void CDemonMove::final_tick()
{
	if (m_pTarget == nullptr)
		return;

	if (check_len())
	{
		return; 
	}

	else
	{
		if (!m_bActive)
			return;

		if (!rotate())
		{
			return;
		}

		move();
	}
	
}

void CDemonMove::Enter()
{
	CMonsterMove::Enter();
}

void CDemonMove::Exit()
{
	CMonsterMove::Exit();
}

bool CDemonMove::check_dir()
{
	//플레이어 와 몬스터 방향 벡터
	//몬스터 z 방향 벡터 내적 

	Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

	Vec3 vDir = (vTargetPos - vPos).Normalize();
	Vec3 vFoward = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	vFoward.z *= -1;

	float fCos = vDir.Dot(vFoward);
	if (vDir.y < 0)
		return false;
	//반대 방향
	if (fCos < 0)
		return false;
	
	//외적 이용 오른쪽 왼쪽 판별
	//Vec3 vCross = vFoward.Cross(vDir);

	//if (vCross.y >= 0)
	//	fRadian = acos(fCos);
	//else
	//	fRadian = 0.f;// XM_PI - acos(fCos);

	

	float fRadian = acos(fCos);

	//float fMaxCos = cos(m_fMaxRadian);
	
	//xz축 플레이와 몬스터가 마주보고 있는지
	//범위안에 각도만 들어오게
	vFoward.y = 0.f;
	vDir.y = 0.f;
	vDir.Normalize();
	float xzRadian = vDir.Dot(vFoward);
	
	//
	if (0.75f > xzRadian || 0.9f <xzRadian)
		return false;

	//80?
	if (fRadian >= m_fMaxRadian)
		return true;

	return false;
}

bool CDemonMove::check_len()
{
	Vec3 vTargetPos = m_pTarget->Collider3D()->GetWorldPos();

	Vec3 vPos = GetOwner()->Collider3D()->GetWorldPos();
	
	Vec3 vLen = vTargetPos - vPos;

	Vec2 vTargetXY = Vec2(vTargetPos.x, vTargetPos.z);
	Vec2 vXY = Vec2(vPos.x, vPos.z);

	float fLen = (vTargetXY - vXY).Length(); //xy축으로만 계산
	float fAttackLen = 600.f;


	if (fLen >= m_fCheckLen)
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
		return true;
	}

	//플레이어가 나보다 위에 있다면
	if (vTargetPos.y >= vPos.y)
	{
		//내 위로 각도 검사
		if ((check_dir()))
		{
			ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::WAIT);
			return true;
		}
	}

	else
	{
		if (fLen <= fAttackLen)
		{
			//각도 계산 캐릭터가 위에 있는지
			ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::ATTACK);//wait
			return true;
		}
	}

	return false;
}
