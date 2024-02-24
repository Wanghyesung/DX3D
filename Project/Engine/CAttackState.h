#pragma once
#include "CState.h"


class CAttackState : public CState
{
private:
	//���° ��������
	wstring m_strAttackNum;
	vector<tAttackInfo> m_vecAttack;
	vector<queue<CGameObject*>> m_vecAttackObj;

	UINT m_iAttackCount; //���� Ƚ��
	UINT m_iCurAttack; //���� �ε��� 
	tAttackInfo m_tCurAttack; //���� ����
	CGameObject* m_pCurGameObj;//���� ��ȯ�� ���ݿ�����Ʈ

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

