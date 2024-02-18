#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterIdle : public CMonsterState
{
private:
	//Å½»ö ¹üÀ§
	float m_fCheckLen;
	float m_fStopLen;

protected:
	CGameObject* m_pTarget;

public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

	void SetCheckLen(float _fLen) { m_fCheckLen = _fLen; }
	float GetCheckLen() { return m_fCheckLen; }

	void SetStopLen(float _fLen) { m_fStopLen = _fLen; }
	float GetStopLen() { return m_fStopLen; }
private:
	virtual void check_player();

public:
	CMonsterIdle();
	virtual ~CMonsterIdle();

};
