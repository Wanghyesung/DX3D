#pragma once
#include "CSingleton.h"



class CEventMgr :
    public CSingleton< CEventMgr>
{
    SINGLE(CEventMgr);
private:
    vector<tEvent>                  m_vecEvent;
    vector<CGameObject*>            m_vecGC;//삭제 오브젝트
    vector<pair<int, CGameObject*>> m_vecEC;//씬에서만 제거

    bool                    m_LevelChanged;

public:
    void AddEvent(const tEvent& _evn) { m_vecEvent.push_back(_evn); }
    bool IsLevelChanged() { return m_LevelChanged; }

private:
    void GC_Clear();
    void EC_Clear();
public:
    void tick();
};

