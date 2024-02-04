#pragma once
#include "CMonsterState.h"

class CMonsterAttack : public CMonsterState
{
public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

private:
	tAttackInfo m_tAttackInfo;

public:
	CMonsterAttack();
	virtual ~CMonsterAttack();

};

