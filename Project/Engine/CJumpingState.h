#pragma once
#include "CState.h"

class CJumpingState : public CState
{
private:
	CGameObject* m_pJumpAttackObj;

	bool m_bJumpAttack;

	Vec3 m_vFinalForce;

public:
	virtual void final_tick() override;
	void Enter()override;
	void Exit()override;

	void SetFinalFoce(const Vec3& _vFinalForce) { m_vFinalForce = _vFinalForce; }
	void SetAttackObj(CGameObject* _pJumpAttack) { m_pJumpAttackObj = _pJumpAttack; }
	void EraseAttack();
public:
	CJumpingState();
	virtual ~CJumpingState();
};

