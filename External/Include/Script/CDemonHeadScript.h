#pragma once
#include <Engine\CScript.h>

class CDemonHeadScript : public CScript
{
private:
    class CGameObject* m_pDemonObject;
    UINT m_iHeadBoneIdx;

    Vec3 m_vHeadOffsetPos;

public:
    virtual void tick() override;
    virtual void begin()override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;

    CLONE(CDemonHeadScript);

public:
    void SetDemonObject(CGameObject* _pDemonObj) { m_pDemonObject = _pDemonObj; }


public:
    CDemonHeadScript();
    virtual ~CDemonHeadScript();
};

