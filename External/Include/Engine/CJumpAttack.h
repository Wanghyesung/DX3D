#pragma once
#include "CState.h"


class CJumpAttack : public CState
{
private:
	CGameObject* m_pTarget;

	tAttack m_tAttack;

	float m_fAttackLen;//���� ����

	float m_fCheckTime;//�� Ž�� �ð�
	float m_fCurTime; //���� �ð�

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

