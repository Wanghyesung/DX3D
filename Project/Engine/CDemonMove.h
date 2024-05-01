#pragma once
#include "CMonsterMove.h"

class CDemonMove : public CMonsterMove
{

private:
	Vec3 m_vDir;
	//float m_fCheckYLen;
	float m_fMaxRadian;
	//�÷��̾�� ���ͱ��� ���� ����

	float m_fMaxDegree;
	//xz������ ���ֺ��� �ִ� ����

private:
	bool check_dir();

protected:
	virtual bool check_len() override;

public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;


public:
	CDemonMove();
	virtual ~CDemonMove();

};

