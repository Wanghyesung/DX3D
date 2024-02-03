#pragma once
#include "CMonsterState.h"

class CGameObject;
class CMonsterIdle : public CMonsterState
{
private:
	CGameObject* m_pTarget;

	//Å½»ö ¹üÀ§
	float m_fCheckLen;

public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;

	void SetCheckLen(float _fLen) { m_fCheckLen = _fLen; }

private:
	void check_player();

public:
	CMonsterIdle();
	virtual ~CMonsterIdle();

};
