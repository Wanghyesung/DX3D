#pragma once
#include "CState.h"

class CJumpState : public CState
{
private:
	int m_iJumpFrame; //���� ���� ������
	int m_iEndFrame; //���� ������

	Vec3 m_vJumpVel;//���� �ӵ�
	Vec3 m_vMaxPos; //�ִ� ���� ����
	Vec3 m_vStartPos; //���� ���� ����

	

	bool m_bAttack;

	//�ݶ��̴��� offset�� �÷�������
	//transform�� ���� �� �ް�
public:
	virtual void final_tick() override;
	void Enter()override;
	void Exit()override;

private:
	void addForce(UINT _iFrame);

	bool check_pos();
public:
	CJumpState();
	virtual ~CJumpState();
};

