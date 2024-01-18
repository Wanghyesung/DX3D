#pragma once
#include "CState.h"

class CWalkState : public CState
{
private:
	virtual void final_tick()override;

	virtual void Enter()override;
	virtual void Exit()override;


public:
	CWalkState();
	virtual ~CWalkState();
};

