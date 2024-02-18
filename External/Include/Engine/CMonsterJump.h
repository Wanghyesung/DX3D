#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterJump : public CMonsterState
{
private:
	CGameObject* m_pTarget;

	//Å½»ö ¹üÀ§
	float m_fCheckLen;
	float m_fStopLen;

	int m_iJumpFrame;
public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

	void SetCheckLen(float _fLen) { m_fCheckLen = _fLen; }

	void SetJumpFrame(int _iFrame) { m_iJumpFrame = _iFrame; }

private:
	void check_player();
public:
	CMonsterJump();
	virtual ~CMonsterJump();

};
