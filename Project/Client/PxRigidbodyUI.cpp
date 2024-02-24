#include "pch.h"
#include "PxRigidbodyUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CPxRigidbody.h>
PxRigidbodyUI::PxRigidbodyUI() :
	ComponentUI("##PxRigidbody", COMPONENT_TYPE::PXRIGIDBODY)
{
	SetName("PxRigidbody");
}


PxRigidbodyUI::~PxRigidbodyUI()
{
}

int PxRigidbodyUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	Vec3 vPos = GetTarget()->PxRigidbody()->GetPxPosition(); 
	
	Vec3 vRotation = GetTarget()->PxRigidbody()->GetPxRotate();
	
	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Position", vPos);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", vRotation);

	GetTarget()->PxRigidbody()->SetPxTransform(vPos);

	PxQuat xRotation(vRotation.x, PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat yRotation(vRotation.y, PxVec3(0.0f, 1.0f, 0.0f));
	PxQuat zRotation(vRotation.z, PxVec3(0.0f, 0.0f, 1.0f));
	
	xRotation *= yRotation;
	xRotation *= zRotation;
	GetTarget()->PxRigidbody()->SetPxRotate(xRotation);
	//
}

