#pragma once
#include "CMonsterIdle.h"

class CGameObject;
class CDemonIdle : public CMonsterIdle
{
private:

public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

	
private:
	virtual void check_player()override;

public:
	CDemonIdle();
	virtual ~CDemonIdle();

};
