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

	STATE_TYPE m_ePrevtateTypes;
public:
	void final_tick();

public:
	void SetOwner(CGameObject* _pObj) { m_pOwner = _pObj; }
	CGameObject* GetOwner() { return m_pOwner; }

	CState* FindState(STATE_TYPE _eType);
	STATE_TYPE GetCurStateType();
	STATE_TYPE GetPrevStateType() { return m_ePrevtateTypes; }

	void ChanageState(STATE_TYPE _eType);
	void AddState(STATE_TYPE _eType, CState* _pState);
	void SetState(STATE_TYPE _eType);

	void SetDir(const wstring& _strDir) { m_strDir = _strDir; }
	const wstring& GetDir() { return m_strDir; }

	void AddAttack(tAttackInfo _tAttackInfo, CGameObject* _pAttackObj);
	
	template <typename T>
	T* GetState()
	{
		map<STATE_TYPE, CState*>::iterator iter =
			m_mapState.begin();

		for (iter; iter != m_mapState.end(); ++iter)
		{
			T* pState = dynamic_cast<T*>(iter->second);
			if (pState != nullptr)
				return pState;
		}

		return nullptr;
	}

	CLONE_DISABLE(CFSM);
public:
	CFSM();
	virtual ~CFSM();

};

