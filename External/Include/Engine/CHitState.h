#pragma once
#include "CState.h"

class CHitState : public CState
{
private:
	bool m_bReverse;

	tHitInfo m_tHitInfo;

	float m_fCurFoceTime;

private:
	void add_foce();

public:
	void SetHitInfo(const tHitInfo& _tHitInfo) { m_tHitInfo = _tHitInfo; }

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

	CLONE_DISABLE(CHitState)
public:
	CHitState();
	virtual ~CHitState();
};

