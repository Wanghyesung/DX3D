#pragma once
#include <Engine\CScript.h>

class CMonsterScript :
    public CScript
{
private:

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void Initialize(const wstring& _strFbxName);

    void AddAnimFrame(const wstring& _strAnimName, int _iStart, int _iEnd);
    CLONE(CMonsterScript);

  
public:
    CMonsterScript();
    ~CMonsterScript();
};


