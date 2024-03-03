#pragma once
#include "CAttackScript.h"

class CJumpAttackScript :
    public CAttackScript
{

public:
    virtual void tick() override;
    virtual void begin()override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;

private:
    CLONE(CJumpAttackScript);

public:
    CJumpAttackScript();
    ~CJumpAttackScript();
};



