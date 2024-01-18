#pragma once
#include "CEntity.h"

class CFSM;

class CState : public CEntity
{
private:
	CFSM* m_pFSM;

	STATE_TYPE m_eType;
public:
	virtual void final_tick() = 0;
	virtual void Exit() = 0;
	virtual void Enter() = 0;

	void Chanage_Anim(const wstring& _strName, bool _bRepeat = true);

	const STATE_TYPE& GetType() { return m_eType; }
	void SetType(const STATE_TYPE& _eType) { m_eType = _eType; }

	CGameObject* GetOwner();
	CFSM* GetFSM() { return m_pFSM; }

	CLONE_DISABLE(CState);
public:
	CState();
	virtual ~CState();

	friend class CFSM;
};

