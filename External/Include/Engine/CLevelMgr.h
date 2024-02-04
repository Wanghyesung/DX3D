#pragma once
#include "CSingleton.h"
#include "CLevel.h"
class CGameObject;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{   
    SINGLE(CLevelMgr);
private:
    CLevel*     m_pCurLevel;

    map<wstring, CLevel*> m_mapLevel;
public:
    CLevel* GetCurLevel() { return m_pCurLevel; }
    CGameObject* FindObjectByName(const wstring& _strName);

    void FindObjectByName(const wstring& _strName, vector<CGameObject*>& _Out);
    void EraseObject(int _iLayer, CGameObject* _pGameObj);

    CLevel* FindLevel(const wstring& _strName);
    void ChangeLevel(CLevel* _pLevel);

    void AddLevel(CLevel* _pLevel);
public:
    void init();
    void tick();
};

