#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterMove : public CMonsterState
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

