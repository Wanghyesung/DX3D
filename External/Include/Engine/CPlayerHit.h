#pragma once
#include "CState.h"

class CPlayerHit : public CState
{
private:
	tHitInfo m_tHitInfo;

public:
	void SetHitInfo(const tHitInfo& _tHitInfo) { m_tHitInfo = _tHitInfo; }

public:
	virtual void final_tick()override;
	virtual void Exit() override;
	virtual void Enter() override;

public:
	CPlayerHit();
	virtual ~CPlayerHit();

};

