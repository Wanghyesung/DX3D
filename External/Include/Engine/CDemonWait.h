#pragma once
#include "CMonsterState.h"

class CDemonWait : public CMonsterState
{

private:
	CGameObject* m_pTarget;

	bool m_bJumpOn;

	float m_fWaitTime;
	float m_fCurTime;

	float m_fCheckLen;

	//�ڷ� �̵��� �Ÿ� //z�����θ� �̵�
	float m_fMoveLen;
	float m_fStartLen;

private:
	void rotate();

	bool check_len();

	void move_back();
public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	CDemonWait();
	virtual ~CDemonWait();
};

