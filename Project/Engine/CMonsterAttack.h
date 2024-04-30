#pragma once
#include "CMonsterState.h"

class CMonsterAttack : public CMonsterState
{
private:
	vector<tAttackInfo> m_vecAttack;
	vector<queue<CGameObject*>> m_vecAttackObj;

	UINT m_iAttackCount; //���� Ƚ��
	UINT m_iCurAttack; //���� �ε��� 
	tAttackInfo m_tCurAttack; //���� ����
	CGameObject* m_pCurGameObj;//���� ��ȯ�� ���ݿ�����Ʈ
	
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

