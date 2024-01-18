#pragma once
#include "CState.h"

struct tAttackInfo
{
	int iAttackNum;

	//�����Ҷ� ������
	float fForce;

	//�������� ����������
	int iStartFrame;
	int iEndFrame;

	//���� ��Ʈ�ڽ�
	Vec3 vAttackScale;
	Vec3 vAttackPos;
	Vec3 vAttackRot;

	//���������
};

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

