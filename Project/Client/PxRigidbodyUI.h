#pragma once
#include "ComponentUI.h"

class CGameObject;

class PxRigidbodyUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    PxRigidbodyUI();
    ~PxRigidbodyUI();
};

