#pragma once
#include <Engine\CScript.h>

class CLightMoveScript :
    public CScript
{
private:
    CGameObject* m_pTarget;

    float       m_fCamSpeed;
    float       m_fMaxDistance;

    bool        m_bCameMove;

public:
    virtual void tick() override;
    virtual void begin() override;
    void SetTarget(CGameObject* _pGameObject) { m_pTarget = _pGameObject; }

private:
    void Light3DMove();

    CLONE(CLightMoveScript);


private:
    void move_target();
    void move_editor();

public:
    CLightMoveScript();
    ~CLightMoveScript();
};

