#pragma once
#include "CMonsterState.h"

class CDemonHold : public CMonsterState
{
private:
	float m_fAttackTime;
	float m_fAttackCurTime;

	float m_fCheckTime;
	float m_fCheckCurTime;

	CGameObject* m_pTarget;
public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

private:
	void check_player();

	void attack();
public:
	CDemonHold();
	virtual ~CDemonHold();
};

