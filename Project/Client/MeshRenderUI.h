#pragma once
#include "ComponentUI.h"

class MeshRenderUI :
    public ComponentUI
{
private:
    bool m_bRenderShadow;
    

public:
    virtual int render_update() override;

   
public:
    void SelectMesh(DWORD_PTR _Key);
    void SelectMaterial(DWORD_PTR _Key);


public:
    MeshRenderUI();
    ~MeshRenderUI();
};

