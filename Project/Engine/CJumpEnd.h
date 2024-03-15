#pragma once
#include "CState.h"


class CJumpEnd : public CState
{
private:
	
public:
	virtual void final_tick() override;
	void Enter()override;
	void Exit()override;

public:
	CJumpEnd();
	virtual ~CJumpEnd();
};

