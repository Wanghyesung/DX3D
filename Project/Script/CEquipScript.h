#pragma once
#include <Engine\CScript.h>
class CEquipScript :
    public CScript
{
private:
    CGameObject* m_pTarget;

    vector<Matrix> m_vecFinalBone;
    Matrix m_matFinalBone;

    UINT m_iIndex;

private:
    bool check_matrix();

public:
    void SetTarget(CGameObject* _pTarget) { m_pTarget = _pTarget; }

public:
    virtual void tick() override;
    virtual void begin()override;
private:
    CLONE(CEquipScript);
public:
    CEquipScript();
    ~CEquipScript();
};

