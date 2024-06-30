#include "PointLightComponent.h"

#include <sstream>

#include "ECS/ECS.h"
#include "Globes.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

CPointLightComponent::CPointLightComponent()
	: Constant{ -1.f }, Linear{ -1.f }, Quadratic{ -1.f }
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);
	Position = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

void CPointLightComponentSystem::UpdatePointLights()
{
	if (IsCountDirty)
	{
		LShader::SetGlobalInt("ActivePointLights", PointLightCount);
		IsCountDirty = false;
	}

	assert(ECS);
	assert(PointLightCount <= MAX_NUM_POINT_LIGHT);

	for (int i = 0; i < PointLightCount; ++i)
	{
		if (CPointLightComponent* PointLightComp = ECS->GetComponent<CPointLightComponent>(PointLights[i]))
		{
			UpdatePointLight(PointLightComp, i);
		}
	}
}

void CPointLightComponentSystem::OnComponentAdded(CPointLightComponent& Component)
{
	if (PointLightCount < MAX_NUM_POINT_LIGHT)
	{
		++PointLightCount;
		IsCountDirty = true;
	}

	PointLights.push_back(Component.EntityID);
}

void CPointLightComponentSystem::OnComponentRemoved(CPointLightComponent& Component)
{
	std::vector<LEntityID>::iterator it = std::find(PointLights.begin(), PointLights.end(), Component.EntityID);
	
	int Index = it - PointLights.begin();

	if (Index >= MAX_NUM_POINT_LIGHT)
	{
		PointLights.erase(PointLights.begin() + Index);
	}
	else
	{
		if (Index == PointLights.size() - 1)
		{
			PointLights.erase(PointLights.begin() + Index);
			--PointLightCount;
			IsCountDirty = true;
		}
		else
		{
			PointLights[Index] = PointLights[PointLights.size() - 1];

			assert(ECS);
			CPointLightComponent* PointLightComp = ECS->GetComponent<CPointLightComponent>(PointLights[Index]);
			assert(PointLightComp);
			
			if (PointLights.size() <= MAX_NUM_POINT_LIGHT)
			{
				--PointLightCount;
				IsCountDirty = true;
			}

			PointLights.pop_back();
		}
	}
}

void CPointLightComponentSystem::UpdatePointLight(CPointLightComponent* Component, int Index)
{
	assert(Component);
	assert(Index >= 0 && Index <= PointLightCount);
	
	CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(Component->EntityID);

	std::stringstream PointLightElement;
	PointLightElement << "PointLights[" << Index << "].";

	LLighting::UpdateLightComponentVec(Component->Position, TransformComp->GetPosition(), "Position", PointLightElement);
	LLighting::UpdateLightComponentVec(PointLightCache[Index].Ambient, Component->Ambient, "Ambient", PointLightElement);
	LLighting::UpdateLightComponentVec(PointLightCache[Index].Diffuse, Component->Diffuse, "Diffuse", PointLightElement);
	LLighting::UpdateLightComponentVec(PointLightCache[Index].Specular, Component->Specular, "Specular", PointLightElement);

	LLighting::UpdateLightComponentFloat(PointLightCache[Index].Constant, Component->Constant, "Constant", PointLightElement);
	LLighting::UpdateLightComponentFloat(PointLightCache[Index].Linear, Component->Linear, "Linear", PointLightElement);
	LLighting::UpdateLightComponentFloat(PointLightCache[Index].Quadratic, Component->Quadratic, "Quadratic", PointLightElement);
}
