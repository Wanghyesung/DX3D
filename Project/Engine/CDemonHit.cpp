#include "pch.h"
#include "CDemonHit.h"
#include "CGameObject.h"
#include "CAnimation3D.h"

CDemonHit::CDemonHit()
{

}

CDemonHit::~CDemonHit()
{

}

void CDemonHit::final_tick()
{
	CGameObject* pObj = GetOwner()->GetChild().at(0);
	bool bCompplete = pObj->Animator3D()->GetCurAnim()->IsComplete();

	if (bCompplete)
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
}

void CDemonHit::Exit()
{

}

void CDemonHit::Enter()
{
	if (m_tHitInfo.bDown)
	{
		Chanage_Anim(GetName(), false);
	}
}



