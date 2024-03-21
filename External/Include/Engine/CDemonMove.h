#pragma once
#include "CMonsterMove.h"

class CDemonMove : public CMonsterMove
{

private:
	Vec3 m_vDir;
	float m_fCheckLen;

private:
	bool check_dir();

public:
	virtual void final_tick()override;
	virtual void Enter()override;
	virtual void Exit()override;


public:
	CDemonMove();
	virtual ~CDemonMove();

};

