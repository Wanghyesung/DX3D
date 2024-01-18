#pragma once
#include <Engine\CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    float       m_fCamSpeed;

public:
    virtual void tick() override;

    void SetTarget(CGameObject* _pGameObject) { m_pTarget = _pGameObject; }

private:
    void Camera2DMove();
    void Camera3DMove();

    CLONE(CCameraMoveScript);

    CGameObject* m_pTarget;

private:
    void move_target();
    void move_editor();

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

