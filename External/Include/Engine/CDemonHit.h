#pragma once
#include "CMonsterHit.h"

class CDemonHit : public CMonsterHit
{

public:
	virtual void final_tick()override;
	virtual void Exit()override;
	virtual void Enter()override;

public:
	CDemonHit();
	virtual ~CDemonHit();

};


