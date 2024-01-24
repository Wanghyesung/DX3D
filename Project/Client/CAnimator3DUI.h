#pragma once
#include "ComponentUI.h"

class CAnimator3DUI : public ComponentUI
{
public:
    virtual int render_update() override;

public:
    CAnimator3DUI();
    ~CAnimator3DUI();

};