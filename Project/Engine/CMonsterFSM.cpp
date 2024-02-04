#include "pch.h"
#include "CMonsterFSM.h"

#include "CMonsterState.h"


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

void CMonsterFSM::AddMonsterAttack(const tAttackInfo& _tAttackInfo)
{
	if (_tAttackInfo.iAttackNum > m_vecAttackInfo.size())
	{
		m_vecAttackInfo.resize(_tAttackInfo.iAttackNum + 1);
	}

	m_vecAttackInfo[_tAttackInfo.iAttackNum] = _tAttackInfo;
}

const tAttackInfo& CMonsterFSM::GetAttackInfo(UINT _iNum)
{
	if (m_vecAttackInfo.size() < _iNum)
		assert(nullptr);

	return m_vecAttackInfo[_iNum];
}


