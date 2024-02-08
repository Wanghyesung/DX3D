#include "pch.h"
#include "CMonsterFSM.h"

#include "CMonsterState.h"
#include "CMonsterAttack.h"

#include "CTransform.h"
#include "CCollider3D.h"
CMonsterFSM::CMonsterFSM()
{

}

CMonsterFSM::~CMonsterFSM()
{
	map<MONSTER_STATE_TYPE, CMonsterState*>::iterator iter =
		m_mapState.begin();
	
	for (; iter != m_mapState.end(); ++iter)
	{
		delete iter->second;
		iter->second = nullptr;
	}
}

void CMonsterFSM::final_tick()
{
	if (!m_pCurState)
		return;

	m_pCurState->final_tick();
}

CMonsterState* CMonsterFSM::FindState(MONSTER_STATE_TYPE _eType)
{
	map<MONSTER_STATE_TYPE, CMonsterState*>::iterator iter =
		m_mapState.find(_eType);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

MONSTER_STATE_TYPE CMonsterFSM::GetCurStateType()
{
	return m_pCurState->GetType();
}

void CMonsterFSM::ChanageState(MONSTER_STATE_TYPE _eType)
{
	CMonsterState* pState = FindState(_eType);

	if (pState == nullptr)
		return;

	m_pCurState->Exit();

	m_pCurState = pState;

	m_pCurState->Enter();
}

void CMonsterFSM::AddState(MONSTER_STATE_TYPE _eType, CMonsterState* _pState)
{
	CMonsterState* pState = FindState(_eType);

	//ม฿บน state
	if (pState != nullptr)
		assert(nullptr);

	_pState->m_pFSM = this;
	_pState->SetType(_eType);
	m_mapState.insert(make_pair(_eType, _pState));
}

void CMonsterFSM::SetState(MONSTER_STATE_TYPE _eType)
{
	CMonsterState* pState = FindState(_eType);

	if (pState == nullptr)
		return;

	m_pCurState = pState;
}

void CMonsterFSM::AddMonsterAttack(const tAttackInfo& _tAttackInfo , CGameObject* _pAttckObj)
{
	CMonsterState* pState = FindState(MONSTER_STATE_TYPE::ATTACK);
	CMonsterAttack* pAttack = dynamic_cast<CMonsterAttack*>(pState);

	pAttack->AddAttack(_tAttackInfo, _pAttckObj);
}


