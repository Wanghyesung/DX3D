#include "pch.h"
#include "CAnimator3DUI.h"


#include <Engine\CGameObject.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CAnimation3D.h>

CAnimator3DUI::CAnimator3DUI()
    : ComponentUI("##CAnimator3DUI", COMPONENT_TYPE::ANIMATOR3D)
{
    SetName("CAnimator3DUI");
}

CAnimator3DUI::~CAnimator3DUI()
{
}

int CAnimator3DUI::render_update()
{
   if (FALSE == ComponentUI::render_update())
       return FALSE;
   //
   CAnimator3D* pAnim = GetTarget()->Animator3D();
   //
   ImGui::Text("STOP");
   ImGui::SameLine();
   bool bOn = ImGui::SmallButton("STOP");
   bool bStop = pAnim->IsStop();
   if (bOn)
   {
       bStop = !bStop;
       pAnim->Stop(bStop);
   }
   
    const wstring& strName = pAnim->GetCurAnim()->GetName();
    string name = string(strName.begin(), strName.end());
    string finalname = "Cur Animation : " + name;
    ImGui::Text(finalname.c_str());
    ImGui::SameLine();

    int inum = 0;
    int iCurFrame = pAnim->GetCurAnim()->GetCurFrame();
    ImGui::Text("Cur Frame : ");
    ImGui::SameLine();
    ImGui::DragInt("##Frame", &inum);
    if (bStop && iCurFrame != inum)
    {
        pAnim->SetCurFrame(inum);
    }

    


    return TRUE;
}
