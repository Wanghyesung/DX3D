#pragma once
#include <Engine\CScript.h>

class CMonsterFSM;
class CMonsterState;

class CMonsterScript :
    public CScript
{
private:
    CMonsterFSM* m_pFSM;

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void begin();

    void Initialize(const wstring& _strFbxName);

    void AddAnimFrame(const wstring& _strAnimName, int _iStart, int _iEnd);
    CLONE(CMonsterScript);

public:
    void AddMonsterState(MONSTER_STATE_TYPE _eType, CMonsterState* _pState,
                         const wstring& _strAnimName, int _iStartFrame, int _iEndFrame);
  
public:
    CMonsterScript();
    ~CMonsterScript();
};


