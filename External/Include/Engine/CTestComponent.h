#pragma once
#include "CComponent.h"

class CTestComponent : public CComponent
{
private:
    class CGameObject* m_pDemonObject;
    UINT m_iHeadBoneIdx;

    Vec3 m_vHeadOffsetPos;

public:
    virtual void begin()override;
    virtual void finaltick()override;

    CLONE(CTestComponent);

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);


public:
    void SetDemonObject(CGameObject* _pDemonObj) { m_pDemonObject = _pDemonObj; }


public:
    CTestComponent();
    virtual ~CTestComponent();
};

