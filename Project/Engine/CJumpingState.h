#pragma once
#include "CState.h"

class CJumpingState : public CState
{
private:

public:
	virtual void final_tick() override;
	void Enter()override;
	void Exit()override;

public:
	CJumpingState();
	virtual ~CJumpingState();
};

