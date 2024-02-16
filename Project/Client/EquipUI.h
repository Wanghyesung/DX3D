#pragma once
#include "ComponentUI.h"

class EquipUI : public ComponentUI
{
public:
    virtual int render_update() override;

public:
    EquipUI();
    ~EquipUI();

};