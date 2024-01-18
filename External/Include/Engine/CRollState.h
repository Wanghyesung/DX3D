#pragma once
#include "CState.h"

class CRollState : public CState
{

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();


public:
	CRollState();
	virtual ~CRollState();
};

