#pragma once
#include "CState.h"
class CRunState : public CState
{
private:
	virtual void final_tick()override;

	virtual void Enter()override;
	virtual void Exit()override;


public:
	CRunState();
	virtual ~CRunState();

};

