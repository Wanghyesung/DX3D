#include "pch.h"
#include "CDemonMove.h"
#include "CNavMesh.h"
#include "CPxRigidbody.h"

CDemonMove::CDemonMove():
	m_fCheckLen(800.f)
{

}

CDemonMove::~CDemonMove()
{

}


void CDemonMove::final_tick()
{
	CMonsterMove::final_tick();

	//bool bActive = GetOwner()->NavMesh()->IsActive();
	//
	//if (!bActive)
	//{
	//	//플레이어가 위에 있다면 플레이어 쪽을 바라보도록
	//	if (check_dir())
	//	{
	//		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::WAIT);
	//	}
	//}
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
	Vec3 vTargetPos = m_pTarget->PxRigidbody()->GetPxPosition();
	Vec3 vPos = GetOwner()->PxRigidbody()->GetPxPosition();

	Vec3 vDiff = vTargetPos - vPos;

	if (vDiff.y > 800.f)
	{
		Vec3 vDir = vDiff.Normalize();

		return true;
	}
		

	return false;
}