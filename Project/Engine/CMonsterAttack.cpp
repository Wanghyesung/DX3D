#include "pch.h"
#include "CMonsterAttack.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CMonsterFSM.h"
CMonsterAttack::CMonsterAttack()
{

}

CMonsterAttack::~CMonsterAttack()
{

}

void CMonsterAttack::final_tick()
{
	CGameObject* pObj = GetOwner()->GetChild().at(0);

	CAnimation3D* pAnim = pObj->Animator3D()->GetCurAnim();
	
	if (pAnim->IsComplete())
	{
		ChanageMonsterState(GetFSM(), MONSTER_STATE_TYPE::IDLE);
	}

}

void CMonsterAttack::Exit()
{

}

void CMonsterAttack::Enter()
{
	//·£´ý °ø°Ý
	std::random_device rDiv;
	std::mt19937 en(rDiv());
	std::uniform_int_distribution<int> num(0, 1);
	UINT iAttackNum = (UINT)num(en);

	wstring strName = GetName() + std::to_wstring(iAttackNum);
	Chanage_Anim(strName, false);

	m_tAttackInfo = GetFSM()->GetAttackInfo(iAttackNum);
}


