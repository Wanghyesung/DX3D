#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterMove : public CMonsterState
{
protected:
	CGameObject* m_pTarget;

	float m_fAttackLen;
	float m_fCheckLen;
	float m_fStopLen;


public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

	float GetStopLen() { return m_fStopLen; }
	float GetAttackLen() { return m_fAttackLen; }

	void SetStopLen(float _fLen) { m_fStopLen = _fLen; }
	void SetAttackLen(float _fLen) { m_fAttackLen = _fLen; }
protected:
	virtual bool check_len();

	bool rotate();
	void move();

	void lookat_player();

public:
	CMonsterMove();
	virtual ~CMonsterMove();

};

