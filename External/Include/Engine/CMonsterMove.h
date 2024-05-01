#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterMove : public CMonsterState
{
protected:
	CGameObject* m_pTarget;

	float m_fCheckLen;
	float m_fStopLen;


	bool m_bActive;


public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

	float GetCheckLen() { return m_fCheckLen; }
	float GetStopLen() { return m_fStopLen; }

	void SetCheckLen(float _fLen) { m_fCheckLen = _fLen; }
	void SetStopLen(float _fLen) { m_fStopLen = _fLen; }

protected:
	virtual bool check_len();

	bool rotate();
	void move();

	void lookat_player();

public:
	CMonsterMove();
	virtual ~CMonsterMove();

};

