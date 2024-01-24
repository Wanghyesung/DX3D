#pragma once
#include "CState.h"

class CGameObject;
class CMonsterMove : public CState
{
private:
	CGameObject* m_pTarget;

public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

public:
	CMonsterMove();
	virtual ~CMonsterMove();

};

