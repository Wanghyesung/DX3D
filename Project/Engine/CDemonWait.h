#pragma once
#include "CMonsterState.h"

class CDemonWait : public CMonsterState
{

private:
	CGameObject* m_pTarget;

	float m_fWaitTime;
	float m_fCurTime;

	float m_fCheckLen;

	//뒤로 이동할 거리
	float m_fMoveLen;
private:
	void rotate();

	bool check_len();
public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	CDemonWait();
	virtual ~CDemonWait();
};

