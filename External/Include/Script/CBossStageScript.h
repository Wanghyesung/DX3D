#pragma once
#include <Engine\CScript.h>

class CBossStageScript : public CScript
{
private:


public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBossStageScript);

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;
public:
    CBossStageScript();
    ~CBossStageScript();

};

