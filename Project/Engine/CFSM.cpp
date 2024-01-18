#include "pch.h"
#include "CFSM.h"
#include "CGameObject.h"
#include "CAnimator3D.h"

void CFSM::final_tick()
{
    if (!m_pCurState)
        return;

    m_pCurState->final_tick();
}


CState* CFSM::FindState(STATE_TYPE _eType)
{
    map<STATE_TYPE, CState*>::iterator iter =
        m_mapState.find(_eType);

    if (iter == m_mapState.end())
        return nullptr;

    return iter->second;
}

STATE_TYPE CFSM::GetCurStateType()
{
    return m_pCurState->GetType();
}


void CFSM::ChanageState(STATE_TYPE _eType)
{
    CState* pState = FindState(_eType);

    if (pState == nullptr)
        return;

    m_pCurState->Exit();

    m_pCurState = pState;

    m_pCurState->Enter();
}

void CFSM::AddState(STATE_TYPE _eType, CState* _pState)
{
    CState* pState = FindState(_eType);

    //ม฿บน state
    if (pState != nullptr)
        assert(nullptr);

    _pState->m_pFSM = this;
    _pState->SetType(_eType);
    m_mapState.insert(make_pair(_eType, _pState));
}

void CFSM::SetState(STATE_TYPE _eType)
{
    CState* pState = FindState(_eType);

    if (pState == nullptr)
        return;

    m_pCurState = pState;
}

void CFSM::AddAttack(tAttackInfo _tAttackInfo)
{
    CAttackState* pAttackState = dynamic_cast<CAttackState*>(FindState(STATE_TYPE::ATTACK));

    pAttackState->AddAttack(_tAttackInfo);
}

CFSM::CFSM() :
    m_pOwner(nullptr),
    m_mapState{},
    m_pCurState(nullptr),
    m_strDir(L"Front")
{
}

CFSM::~CFSM()
{
    map<STATE_TYPE, CState*>::iterator iter =
        m_mapState.begin();

    for (; iter != m_mapState.end(); ++iter)
    {
        delete iter->second;
        iter->second = nullptr;
    }
}

