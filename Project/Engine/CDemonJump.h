#pragma once
#include "CMonsterState.h"
class CDemonJump : public CMonsterState
{
private:
	int m_iJumpStartFrame;

	Vec3 m_vJumpForce;
	Vec3 m_vFinalJumpPos;

	float m_fMaxLen;//��ü �پ�� �� �Ÿ�

	bool m_bAttackTrigger;

private:
	void jump();

public:
	void SetAttackTrigger() { m_bAttackTrigger = true; }

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	CDemonJump();
	virtual ~CDemonJump();


};

