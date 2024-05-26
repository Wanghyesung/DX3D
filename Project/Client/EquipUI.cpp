#include "pch.h"
#include "EquipUI.h"

#include <Engine\CEquip.h>
#include <Engine\CGameObject.h>
EquipUI::EquipUI()
    : ComponentUI("##EquipUI", COMPONENT_TYPE::EQUIP)
{
    SetName("Equip");
}

EquipUI::~EquipUI()
{
}

int EquipUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;


    int iCurIndex = GetTarget()->Equip()->GetIndex();
    ImGui::Text("Cur Index : ");
    ImGui::SameLine();
    ImGui::DragInt("##Frame", &iCurIndex);
   
    Vec3 vFixedPos = GetTarget()->Equip()->GetFixedPos();
    ImGui::Text("Worldpos");
    ImGui::SameLine();
    ImGui::DragFloat3("##FixedPos", vFixedPos);


    GetTarget()->Equip()->SetFixedPos(vFixedPos);
    GetTarget()->Equip()->SetIndex(iCurIndex);

    return TRUE;
}

