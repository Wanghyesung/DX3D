#include "pch.h"
#include "CParticleSystemUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CParticleSystem.h>

CParticleSystemUI::CParticleSystemUI() :
	ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
{
	SetName("ParticleSystem");
}

CParticleSystemUI::~CParticleSystemUI()
{

}

int CParticleSystemUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	tParticleModule tModuleData = GetTarget()->ParticleSystem()->GetParticleMoudle();

	ImGui::Text("SpawnRate");
	ImGui::SameLine();
	ImGui::DragInt("##SpawnRate", &tModuleData.SpawnRate);

	ImGui::Text("Speed");
	ImGui::SameLine();
	ImGui::DragFloat("##Speed", &tModuleData.Speed);

	ImGui::Text("Velocity Type");
	ImGui::SameLine();
	ImGui::InputInt("##Velocity Type", &tModuleData.AddVelocityType);

	ImGui::Text("Velocity Dir");
	ImGui::SameLine();
	ImGui::DragFloat4("##Velocity Dir", tModuleData.vVelocityDir);

	GetTarget()->ParticleSystem()->SetParticleModule(tModuleData);

	return TRUE;
}
