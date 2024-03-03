#pragma once
#include "CState.h"

class CJumpState : public CState
{
private:
	int m_iJumpFrame; //���� ���� ������
	int m_iEndFrame; //���� ������

	Vec3 m_vJumpVel;//���� �ӵ�
	Vec3 m_vMaxPos; //�ִ� ���� ����

	Vec3 m_vBeginOffsetPos; //ó�� pxtransformoffset ��ġ

	int m_iBoneIdx;

	bool m_bAttack;

public:
	virtual void final_tick() override;
	void Enter()override;
	void Exit()override;

	void SetBoneIdx(int _iIdx) { m_iBoneIdx = _iIdx; }

	bool IsAttack() { return m_bAttack; }

private:
	void addForce(UINT _iFrame);
	
	bool check_pos(); //���� ��ġ �˻�
	void check_key(Vec3& _vFinalVel); //Ű �˻�
	void check_attack(); //���� Ű

public:
	CJumpState();
	virtual ~CJumpState();
};

