#pragma once
#include "CMonsterState.h"
class CDemonJump : public CMonsterState
{
private:

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	CDemonJump();
	virtual ~CDemonJump();


};

