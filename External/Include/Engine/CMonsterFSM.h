#pragma once
#include "CEntity.h"

class CMonsterState;

class CMonsterFSM : public CEntity
{
private:
	CGameObject* m_pOwner;

	map<MONSTER_STATE_TYPE, CMonsterState*> m_mapState;

	CMonsterState* m_pCurState;
	wstring m_strDir;

	vector<tAttackInfo> m_vecAttackInfo;

public:
	void final_tick();

public:
	void SetOwner(CGameObject* _pObj) { m_pOwner = _pObj; }
	CGameObject* GetOwner() { return m_pOwner; }

	CMonsterState* FindState(MONSTER_STATE_TYPE _eType);
	MONSTER_STATE_TYPE GetCurStateType();

	void ChanageState(MONSTER_STATE_TYPE _eType);
	void AddState(MONSTER_STATE_TYPE _eType, CMonsterState* _pState);
	void SetState(MONSTER_STATE_TYPE _eType);


	void SetDir(const wstring& _strDir) { m_strDir = _strDir; }
	const wstring& GetDir() { return m_strDir; }

	void AddMonsterAttack(const tAttackInfo& _tAttackInfo);
	
	const tAttackInfo& GetAttackInfo(UINT _iNum);

	CLONE_DISABLE(CMonsterFSM);
public:
	CMonsterFSM();
	virtual ~CMonsterFSM();

};

