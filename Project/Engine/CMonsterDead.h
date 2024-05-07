#pragma once
#include "CMonsterState.h"

class CMonsterDead : public CMonsterState
{
private:


protected:
	tHitInfo m_tHitInfo;

public:
	virtual void final_tick()override;
	virtual void Exit()override;
	virtual void Enter()override;

public:
	void SetHitInfo(const tHitInfo& _tHitInfo) { m_tHitInfo = _tHitInfo; }

public:
	CMonsterDead();
	virtual ~CMonsterDead();

};

