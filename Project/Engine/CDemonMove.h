#pragma once
#include "CMonsterMove.h"

class CDemonMove : public CMonsterMove
{

private:
	Vec3 m_vDir;
	//float m_fCheckYLen;
	float m_fMaxRadian;
	//플레이어에서 몬스터까지 벡터 각도

	float m_fMaxDegree;
	//xz축으로 마주보는 최대 각도

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

