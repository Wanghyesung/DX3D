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
       //자식들 애니메이션도 모두 종료
       const vector<CGameObject*>& vecAnim = pAnim->GetOwner()->GetParent()->GetChild();
       for (int i = 0; i < vecAnim.size(); ++i)
       {
           vecAnim[i]->Animator3D()->Stop(bStop);
       }
   }
   
   ImGui::Text("Reset");
   ImGui::SameLine();
   bOn = ImGui::SmallButton("Reset");
   if (bOn)
   {
       const vector<CGameObject*>& vecAnim = pAnim->GetOwner()->GetParent()->GetChild();
       for (int i = 0; i < vecAnim.size(); ++i)
       {
           vecAnim[i]->Animator3D()->Reset();
       }
   }
   ImGui::NewLine();

   if (!pAnim->GetCurAnim())
       return TRUE;

    const wstring& strName = pAnim->GetCurAnim()->GetName();
    string name = string(strName.begin(), strName.end());
    string finalname = "Cur Animation : " + name;
    ImGui::Text(finalname.c_str());
    //ImGui::SameLine();

    //int inum = 0;
    int iCurFrame = pAnim->GetCurAnim()->GetCurFrame();
    int iTemFrame = iCurFrame;
    ImGui::Text("Cur Frame : ");
    ImGui::SameLine();
    ImGui::DragInt("##Frame", &iCurFrame);
    if (bStop && iCurFrame != iTemFrame)
    {
        const vector<CGameObject*>& vecAnim = pAnim->GetOwner()->GetParent()->GetChild();
        for (int i = 0; i < vecAnim.size(); ++i)
        {
            vecAnim[i]->Animator3D()->SetCurFrame(iCurFrame);
        }
    }

    ImGui::NewLine();

    int iCurAnimIdx = pAnim->GetCurAnimIdx();
    int iTemIdx = iCurAnimIdx;
    ImGui::Text("CurAnimIdx : ");
    ImGui::SameLine();
    ImGui::DragInt("##Idx", &iCurAnimIdx);

    if(iCurAnimIdx != iTemIdx)
    {
        const vector<CGameObject*>& vecAnim = pAnim->GetOwner()->GetParent()->GetChild();
        for (int i = 0; i < vecAnim.size(); ++i)
        {
            vecAnim[i]->Animator3D()->SetCurIdx(iCurAnimIdx);
        }
    }

    return TRUE;
}
