#pragma once
#include "CState.h"

class CJumpState : public CState
{
private:
	int m_iJumpFrame; //점프 시작 프레임
	int m_iEndFrame; //착지 프레임

	Vec3 m_vJumpVel;//점프 속도
	Vec3 m_vMaxPos; //최대 점프 지점

	Vec3 m_vBeginOffsetPos; //처음 pxtransformoffset 위치

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
	
	bool check_pos(); //점프 위치 검사
	void check_key(Vec3& _vFinalVel); //키 검사
	void check_attack(); //공격 키

public:
	CJumpState();
	virtual ~CJumpState();
};

