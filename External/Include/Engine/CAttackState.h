#pragma once
#include "CState.h"

struct tAttackInfo
{
	int iAttackNum;

	//공격할때 움직임
	float fForce;

	//공격판정 시작프레임
	int iStartFrame;
	int iEndFrame;

	//공격 히트박스
	Vec3 vAttackScale;
	Vec3 vAttackPos;
	Vec3 vAttackRot;

	//상대적일지
};

class CAttackState : public CState
{
private:
	//몇번째 공격인지
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

