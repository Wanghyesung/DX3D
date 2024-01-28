#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterIdle : public CMonsterState
{
private:
	CGameObject* m_pTarget;

public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

public:
	CMonsterIdle();
	virtual ~CMonsterIdle();

};
