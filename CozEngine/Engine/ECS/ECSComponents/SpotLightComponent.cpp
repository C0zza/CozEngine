#include "SpotLightComponent.h"

#include <sstream>

#include "ECS/ECS.h"
#include "Globes.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

CSpotLightComponent::CSpotLightComponent()
	: CutOff{ -1.f }, OuterCutOff{ -1.f }, Constant{ -1.f }, Linear{ -1.f }, Quadratic{ -1.f }
{
}

void CSpotLightComponentSystem::UpdateSpotLights()
{
	if (IsCountDirty)
	{
		LShader::SetGlobalInt("ActiveSpotLights", SpotLightCount);
		IsCountDirty = false;
	}

	assert(ECS);
	assert(SpotLightCount <= MAX_NUM_SPOT_LIGHT);

	for (int i = 0; i < SpotLightCount; ++i)
	{
		if (CSpotLightComponent* SpotLightComp = ECS->GetComponent<CSpotLightComponent>(SpotLights[i]))
		{
			UpdateSpotLight(SpotLightComp, i);
		}
	}
}

void CSpotLightComponentSystem::OnComponentAdded(CSpotLightComponent& Component)
{
	if (SpotLightCount < MAX_NUM_SPOT_LIGHT)
	{
		++SpotLightCount;
		IsCountDirty = true;
	}

	SpotLights.push_back(Component.EntityID);
}

void CSpotLightComponentSystem::OnComponentRemoved(CSpotLightComponent& Component)
{
	std::vector<LEntityID>::iterator it = std::find(SpotLights.begin(), SpotLights.end(), Component.EntityID);

	int Index = it - SpotLights.begin();

	if (Index >= MAX_NUM_SPOT_LIGHT)
	{
		SpotLights.erase(SpotLights.begin() + Index);
	}
	else
	{
		if (Index == SpotLights.size() - 1)
		{
			SpotLights.erase(SpotLights.begin() + Index);
			--SpotLightCount;
			IsCountDirty = true;
		}
		else
		{
			SpotLights[Index] = SpotLights[SpotLights.size() - 1];

			assert(ECS);
			CSpotLightComponent* SpotLightComp = ECS->GetComponent<CSpotLightComponent>(SpotLights[Index]);
			assert(SpotLightComp);

			if (SpotLights.size() <= MAX_NUM_SPOT_LIGHT)
			{
				--SpotLightCount;
				IsCountDirty = true;
			}

			SpotLights.pop_back();
		}
	}
}

void CSpotLightComponentSystem::UpdateSpotLight(CSpotLightComponent* Component, int Index)
{
	assert(Component);
	assert(Index >= 0 && Index <= SpotLightCount);

	CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(Component->EntityID);

	std::stringstream SpotLightElement;
	SpotLightElement << "SpotLights[" << Index << "].";

	LLighting::UpdateLightComponentVec(Component->Position, TransformComp->GetPosition(), "Position", SpotLightElement);
	LLighting::UpdateLightComponentVec(SpotLightCache[Index].Direction, TransformComp->GetForward(), "Direction", SpotLightElement);
	LLighting::UpdateLightComponentVec(SpotLightCache[Index].Ambient, Component->Ambient, "Ambient", SpotLightElement);
	LLighting::UpdateLightComponentVec(SpotLightCache[Index].Diffuse, Component->Diffuse, "Diffuse", SpotLightElement);
	LLighting::UpdateLightComponentVec(SpotLightCache[Index].Specular, Component->Specular, "Specular", SpotLightElement);

	LLighting::UpdateLightComponentFloat(SpotLightCache[Index].CutOff, Component->CutOff, "CutOff", SpotLightElement);
	LLighting::UpdateLightComponentFloat(SpotLightCache[Index].OuterCutOff, Component->OuterCutOff, "OuterCutOff", SpotLightElement);

	LLighting::UpdateLightComponentFloat(SpotLightCache[Index].Constant, Component->Constant, "Constant", SpotLightElement);
	LLighting::UpdateLightComponentFloat(SpotLightCache[Index].Linear, Component->Linear, "Linear", SpotLightElement);
	LLighting::UpdateLightComponentFloat(SpotLightCache[Index].Quadratic, Component->Quadratic, "Quadratic", SpotLightElement);
}