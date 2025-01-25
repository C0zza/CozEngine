#include "DirectionalLightComponent.h"

#include <cassert>
#include <sstream>

#include "ECS/ECS.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

namespace CE::CDirectionalLightComponent
{
	static const int bIsActiveOffset = 64;
}

CDirectionalLightComponent::CDirectionalLightComponent()
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);
	Direction = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

void CDirectionalLightComponentSystem::OnComponentAdded(CDirectionalLightComponent& Component)
{
	if (!ActiveDirectionalLight)
	{
		// TODO: Setup ECS component handles for future reference handling and serialization
		ActiveDirectionalLight = &Component;
		LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
		Renderer->UpdateLightingUBOData(CE::CDirectionalLightComponent::bIsActiveOffset, sizeof(bool), ActiveDirectionalLight);
		Renderer->UpdateLightingUBOData(0, sizeof(glm::vec3), ActiveDirectionalLight->Direction);
	}
}

void CDirectionalLightComponentSystem::OnComponentRemoved(CDirectionalLightComponent& Component)
{
	if (!ActiveDirectionalLight || ActiveDirectionalLight != &Component)
	{
		return;
	}
	
	CDirectionalLightComponent* TempActiveDirectionalLight = ActiveDirectionalLight;
	ActiveDirectionalLight = nullptr;

	LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();

	static const bool FalseBool = false;
	static const bool TrueBool = true;

	Renderer->UpdateLightingUBOData(CE::CDirectionalLightComponent::bIsActiveOffset, sizeof(bool), FalseBool);
	for (CDirectionalLightComponent& DirectionalLight : GetComponents())
	{
		if (&DirectionalLight != TempActiveDirectionalLight)
		{
			ActiveDirectionalLight = &DirectionalLight;
		}
	}

	if (ActiveDirectionalLight)
	{
		Renderer->UpdateLightingUBOData(CE::CDirectionalLightComponent::bIsActiveOffset, sizeof(bool), TrueBool);
	}
	else
	{
		Renderer->UpdateLightingUBOData(CE::CDirectionalLightComponent::bIsActiveOffset, sizeof(bool), FalseBool);
	}
}

void CDirectionalLightComponentSystem::UpdateDirectionalLight()
{
	if (ActiveDirectionalLight)
	{
		static LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();

		assert(ECS);
		const CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(ActiveDirectionalLight->EntityID);
		assert(TransformComp);

		std::stringstream DirectionalLightVar;
		DirectionalLightVar << "DirectionalLight.";

		if (TransformComp->GetForward() != ActiveDirectionalLight->Direction)
		{
			ActiveDirectionalLight->Direction = TransformComp->GetForward();
			Renderer->UpdateLightingUBOData(0, sizeof(glm::vec3), ActiveDirectionalLight->Direction);
		}

		if (ActiveDirectionalLight->Ambient != CurrentAmbient)
		{
			Renderer->UpdateLightingUBOData(16, sizeof(glm::vec3), ActiveDirectionalLight->Ambient);
			CurrentAmbient = ActiveDirectionalLight->Ambient;
		}

		if (ActiveDirectionalLight->Diffuse != CurrentDiffuse)
		{
			Renderer->UpdateLightingUBOData(32, sizeof(glm::vec3), ActiveDirectionalLight->Diffuse);
			CurrentDiffuse = ActiveDirectionalLight->Diffuse;
		}

		if (ActiveDirectionalLight->Specular != CurrentSpecular)
		{
			Renderer->UpdateLightingUBOData(48, sizeof(glm::vec3), ActiveDirectionalLight->Specular);
			CurrentSpecular = ActiveDirectionalLight->Specular;
		}
	}
}
