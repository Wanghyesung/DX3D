#pragma once
#include "CState.h"
class CIdleState : public CState
{
private:

public:
	virtual void final_tick() override;
	void Enter()override;
	void Exit()override;

public:
	CIdleState();
	virtual ~CIdleState();

};

