#pragma once
#include "CState.h"


class CJumpAttack : public CState
{
private:
	CGameObject* m_pTarget;

	tAttack m_tAttack;

	float m_fAttackLen;//공격 범위

	float m_fCheckTime;//적 탐색 시간
	float m_fCurTime; //현재 시간

private:
	void check_target();
	void check_time();

public:
	void SetAttack(const tAttack& _tAttack) { m_tAttack = _tAttack; }

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	CJumpAttack();
	virtual ~CJumpAttack();
};

