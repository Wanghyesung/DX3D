#pragma once
#include "CEntity.h"

class CMonsterFSM;

class CMonsterState : public CEntity
{
private:
	CMonsterFSM* m_pFSM;

	MONSTER_STATE_TYPE m_eType;
public:
	virtual void final_tick() = 0;
	virtual void Exit() = 0;
	virtual void Enter() = 0;

	void Chanage_Anim(const wstring& _strName, bool _bRepeat = true);

	const MONSTER_STATE_TYPE& GetType() { return m_eType; }
	void SetType(const MONSTER_STATE_TYPE& _eType) { m_eType = _eType; }

	CGameObject* GetOwner();
	CMonsterFSM* GetFSM() { return m_pFSM; }

	CLONE_DISABLE(CMonsterState);
public:
	CMonsterState();
	virtual ~CMonsterState();

	friend class CMonsterFSM;
};

