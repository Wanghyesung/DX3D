#pragma once
#include "CMonsterState.h"

class CMonsterAttack : public CMonsterState
{
private:
	vector<tAttackInfo> m_vecAttack;
	vector<queue<CGameObject*>> m_vecAttackObj;

	UINT m_iAttackCount; //공격 횟수
	UINT m_iCurAttack; //벡터 인덱스 
	tAttackInfo m_tCurAttack; //공격 정보
	CGameObject* m_pCurGameObj;//현재 소환한 공격오브젝트
	
	float m_fCurMoveTime;

public:
	virtual void final_tick();
	virtual void Exit();
	virtual void Enter();

public:
	void AddAttack(tAttackInfo _tAttackInfo, CGameObject* _pAttackObj);
	vector<tAttackInfo>& GetAttackInfo() { return m_vecAttack; }
private:
	void add_force();
	void check_event();

	void add_objpull(UINT _iAttackNum, CGameObject* _pAttackObj);
	
public:
	CMonsterAttack();
	virtual ~CMonsterAttack();

};

