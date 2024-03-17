#pragma once
#include "CState.h"

class CDeadState : public CState
{
private:
	void dead();

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

	CLONE_DISABLE(CDeadState)
public:
	CDeadState();
	virtual ~CDeadState();
};

