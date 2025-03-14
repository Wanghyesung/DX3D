#include "pch.h"
#include "Collider3DUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CCollider3D.h>

int Collider3DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    Vec3 vOffsetPos = GetTarget()->Collider3D()->GetOffsetPos();

    ImGui::Text("OffsetPos");
    ImGui::SameLine();
    ImGui::DragFloat3("##OffsetPos", vOffsetPos);

    Vec3 vOffsetScale = GetTarget()->Collider3D()->GetOffsetScale();
    ImGui::Text("OffsetScale");
    ImGui::SameLine();
    ImGui::DragFloat3("##OffsetScale", vOffsetScale);

    Vec3 vOffsetRot = GetTarget()->Collider3D()->GetOffsetRot();
    ImGui::Text("OffsetRot");
    ImGui::SameLine();
    vOffsetRot = (vOffsetRot / XM_PI) * 180.f;
    ImGui::DragFloat3("##OffsetRot", vOffsetRot);

    bool bIsAbsolute = GetTarget()->Collider3D()->GetIsAbsolute();
    ImGui::Text("Absolute");
    ImGui::SameLine();
    ImGui::Button("##Absolute", ImVec2(20.f, 20.f));

    vOffsetRot = (vOffsetRot / 180.f) * XM_PI;

    GetTarget()->Collider3D()->SetOffsetPos(vOffsetPos);
    GetTarget()->Collider3D()->SetOffsetScale(vOffsetScale);
    GetTarget()->Collider3D()->SetOffsetRot(vOffsetRot);
    GetTarget()->Collider3D()->SetAbsolute(bIsAbsolute);

    return TRUE;
}

Collider3DUI::Collider3DUI()
    : ComponentUI("##Coillider3D", COMPONENT_TYPE::COLLIDER3D)
{
    SetName("Coillider3D");
}
Collider3DUI::~Collider3DUI()
{

}
