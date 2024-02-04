#pragma once
#include "CEntity.h"
#include "CState.h"
#include "CAttackState.h"
class CFSM : public CEntity
{
private:
	CGameObject* m_pOwner;

	map<STATE_TYPE, CState*> m_mapState;

	CState* m_pCurState;
	wstring m_strDir;
public:
	void final_tick();

public:
	void SetOwner(CGameObject* _pObj) { m_pOwner = _pObj; }
	CGameObject* GetOwner() { return m_pOwner; }

	CState* FindState(STATE_TYPE _eType);
	STATE_TYPE GetCurStateType();

	void ChanageState(STATE_TYPE _eType);
	void AddState(STATE_TYPE _eType, CState* _pState);
	void SetState(STATE_TYPE _eType);

	void SetDir(const wstring& _strDir) { m_strDir = _strDir; }
	const wstring& GetDir() { return m_strDir; }


	void AddAttack(tAttackInfo _tAttackInfo, CGameObject* _pAttackObj);

	CLONE_DISABLE(CFSM);
public:
	CFSM();
	virtual ~CFSM();

};

