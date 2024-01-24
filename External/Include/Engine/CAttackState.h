#pragma once
#include "CState.h"


class CAttackState : public CState
{
private:
	//���° ��������
	wstring m_strAttackNum;
	vector<tAttackInfo> m_vecAttack;

	UINT m_iCurAttack;


public:
	void AddAttack(tAttackInfo _tAttackInfo);

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	CAttackState();
	virtual ~CAttackState();
};

