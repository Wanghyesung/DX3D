#pragma once
#include "CState.h"

class CRollState : public CState
{
private:
	Vec3 m_vFoce;

	int m_iStopFrame;

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

	void SetStopFrame(int _iStopFrame) { m_iStopFrame = _iStopFrame; }

public:
	CRollState();
	virtual ~CRollState();
};

