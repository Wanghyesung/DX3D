#pragma once
#include "CMonsterState.h"

class CDemonJumpAttack : public CMonsterState
{
private:
	CGameObject* m_pJumpAttackObj;

	int m_iAttackFrame;
	int m_iAttackEndFrame;
	int m_iFallFrame;

	bool m_bFall;

	//공격 범위와 공격 위치 
	float m_fAttackRange;
	Vec3 m_vAttackOffset;

private:
	void attack();
	void fall();

	void set_fall();
public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

	void SetAttackObj(CGameObject* _pJumpAttack);
	void EraseAttack();

public:
	CDemonJumpAttack();
	virtual ~CDemonJumpAttack();


};