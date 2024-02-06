#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterMove : public CMonsterState
{
private:
	CGameObject* m_pTarget;

	float m_fCheckLen;
	float m_fStopLen;


	bool m_bActive;


public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

private:
	void check_len();

	void rotate();
	void move();

public:
	CMonsterMove();
	virtual ~CMonsterMove();

};

