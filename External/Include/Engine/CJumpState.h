#pragma once
#include "CState.h"

class CJumpState : public CState
{
private:
	int m_iJumpFrame; //점프 시작 프레임
	int m_iEndFrame; //착지 프레임

	Vec3 m_vJumpVel;//점프 속도
	Vec3 m_vMaxPos; //최대 점프 지점
	Vec3 m_vStartPos; //점프 시작 지점

	

	bool m_bAttack;

	//콜라이더는 offset로 올려버리고
	//transform은 영향 안 받게
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

