#pragma once
#include "CState.h"

class CJumpState : public CState
{
private:

public:
	virtual void final_tick() override;
	void Enter()override;
	void Exit()override;

public:
	CJumpState();
	virtual ~CJumpState();
};

