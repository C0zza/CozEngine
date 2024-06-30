#include "DirectionalLightComponent.h"

#include <cassert>
#include <sstream>

#include "ECS/ECS.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

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
		LShader::SetGlobalBool("DirectionalLight.IsActive", true);
		LShader::SetGlobalVec("DirectionalLight.Direction", ActiveDirectionalLight->Direction);
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

	LShader::SetGlobalBool("DirectionalLight.IsActive", false);
	for (CDirectionalLightComponent& DirectionalLight : GetComponents())
	{
		if (&DirectionalLight != TempActiveDirectionalLight)
		{
			ActiveDirectionalLight = &DirectionalLight;
		}
	}

	if (ActiveDirectionalLight)
	{
		LShader::SetGlobalBool("DirectionalLight.IsActive", true);
	}
	else
	{
		LShader::SetGlobalBool("DirectionalLight.IsActive", false);
	}
}

void CDirectionalLightComponentSystem::UpdateDirectionalLight()
{
	if (ActiveDirectionalLight)
	{
		assert(ECS);
		const CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(ActiveDirectionalLight->EntityID);
		assert(TransformComp);

		std::stringstream DirectionalLightVar;
		DirectionalLightVar << "DirectionalLight.";

		if (TransformComp->GetForward() != ActiveDirectionalLight->Direction)
		{
			ActiveDirectionalLight->Direction = TransformComp->GetForward();
			LShader::SetGlobalVec(DirectionalLightVar.str() + "Direction", ActiveDirectionalLight->Direction);
		}

		if (ActiveDirectionalLight->Ambient != CurrentAmbient)
		{
			LShader::SetGlobalVec(DirectionalLightVar.str() + "Ambient", ActiveDirectionalLight->Ambient);
			CurrentAmbient = ActiveDirectionalLight->Ambient;
		}

		if (ActiveDirectionalLight->Diffuse != CurrentDiffuse)
		{
			LShader::SetGlobalVec(DirectionalLightVar.str() + "Diffuse", ActiveDirectionalLight->Diffuse);
			CurrentDiffuse = ActiveDirectionalLight->Diffuse;
		}

		if (ActiveDirectionalLight->Specular != CurrentSpecular)
		{
			LShader::SetGlobalVec(DirectionalLightVar.str() + "Specular", ActiveDirectionalLight->Specular);
			CurrentSpecular = ActiveDirectionalLight->Specular;
		}
	}
}
