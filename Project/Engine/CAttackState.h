#pragma once
#include "CState.h"


class CAttackState : public CState
{
private:
	//몇번째 공격인지
	wstring m_strAttackNum;
	vector<tAttackInfo> m_vecAttack;
	vector<queue<CGameObject*>> m_vecAttackObj;

	UINT m_iAttackCount; //공격 횟수
	UINT m_iCurAttack; //벡터 인덱스 
	tAttackInfo m_tCurAttack; //공격 정보
	CGameObject* m_pCurGameObj;//현재 소환한 공격오브젝트

	float m_fCurMoveTime;
public:
	void AddAttack(tAttackInfo _tAttackInfo, CGameObject* _pAttackObj);

private:
	void add_force();
	void check_event();

	void add_objpull(UINT _iAttackNum, CGameObject* _pAttackObj);

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	CAttackState();
	virtual ~CAttackState();
};

