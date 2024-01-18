#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetName("Transform");
}

TransformUI::~TransformUI()
{
}

int TransformUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetTarget()->Transform()->GetRelativeRot();

	Vec3 vRight = GetTarget()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vUp = GetTarget()->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vFoward = GetTarget()->Transform()->GetRelativeDir(DIR_TYPE::FRONT);

	vRotation = (vRotation / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Position", vPos);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", vRotation);

	ImGui::Text("Right");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Right", vRight);

	ImGui::Text("Up");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Up", vUp);

	ImGui::Text("Foward");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Foward", vFoward);

	GetTarget()->Transform()->SetRelativePos(vPos);
	GetTarget()->Transform()->SetRelativeScale(vScale);

	vRotation = (vRotation / 180.f) * XM_PI;
	GetTarget()->Transform()->SetRelativeRot(vRotation);

	return TRUE;
}
