#include "SpotLightComponent.h"

#include <sstream>

#include "ECS/ECS.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

std::vector<LEntityID> CSpotLightComponent::SpotLightEntityIDs{};
unsigned int CSpotLightComponent::SpotLightCount = 0;
bool CSpotLightComponent::IsCountDirty = true;

CSpotLightComponent::CSpotLightComponent()
	: CutOff{ 0.f }, OuterCutOff{ 0.f }, Constant{ 0.f }, Linear{ 0.f }, Quadratic{ 0.f }
{
}

void CSpotLightComponent::Init()
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);

	Position = ZeroVector;
	Direction = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;

	if (SpotLightCount < MAX_NUM_SPOT_LIGHT)
	{
		SpotLightCount++;
		IsCountDirty = true;
	}

	SpotLightEntityIDs.push_back(EntityID);

	if (SpotLightEntityIDs.size() <= MAX_NUM_SPOT_LIGHT)
	{
		std::stringstream SpotLightElement;
		SpotLightElement << "SpotLights[" << SpotLightEntityIDs.size() - 1 << "].";
		LShader::SetGlobalVec(SpotLightElement.str() + "Position", Position);
		LShader::SetGlobalVec(SpotLightElement.str() + "Direction", Direction);
	}
}

void CSpotLightComponent::Destroy()
{
	std::vector<LEntityID>::iterator it = std::find(SpotLightEntityIDs.begin(), SpotLightEntityIDs.end(), EntityID);

	assert(it != SpotLightEntityIDs.end());

	int Index = it - SpotLightEntityIDs.begin();

	if (Index >= MAX_NUM_SPOT_LIGHT)
	{
		SpotLightEntityIDs.erase(SpotLightEntityIDs.begin() + Index);
	}
	else
	{
		if (Index == SpotLightEntityIDs.size() - 1)
		{
			SpotLightEntityIDs.erase(SpotLightEntityIDs.begin() + Index);
			SpotLightCount--;
			IsCountDirty = true;
		}
		else
		{
			SpotLightEntityIDs[Index] = SpotLightEntityIDs[SpotLightEntityIDs.size() - 1];

			CSpotLightComponent* SpotLightComp = LECS::Get()->GetComponent<CSpotLightComponent>(SpotLightEntityIDs[Index]);
			assert(SpotLightComp);
			SpotLightComp->IsDirty = true;

			if (SpotLightEntityIDs.size() <= MAX_NUM_SPOT_LIGHT)
			{
				SpotLightCount--;
				IsCountDirty = true;
			}

			SpotLightEntityIDs.pop_back();
		}
	}
}

void CSpotLightComponent::SetAmbient(const glm::vec3& i_Ambient)
{
	LLighting::AssertRGBVec(i_Ambient);
	SetDirtyMember(Ambient, i_Ambient);
}

void CSpotLightComponent::SetDiffuse(const glm::vec3& i_Diffuse)
{
	LLighting::AssertRGBVec(i_Diffuse);
	SetDirtyMember(Diffuse, i_Diffuse);
}

void CSpotLightComponent::SetSpecular(const glm::vec3& i_Specular)
{
	LLighting::AssertRGBVec(i_Specular);
	SetDirtyMember(Specular, i_Specular);
}

void CSpotLightComponent::Update(const unsigned int Index)
{
	assert(Index >= 0 && Index <= CSpotLightComponent::SpotLightCount);

	CTransformComponent* TransformComp = LECS::Get()->GetComponent<CTransformComponent>(EntityID);
	assert(TransformComp);

	std::stringstream SpotLightElement;
	SpotLightElement << "SpotLights[" << Index << "].";

	if (TransformComp->GetPosition() != Position)
	{
		Position = TransformComp->GetPosition();
		LShader::SetGlobalVec(SpotLightElement.str() + "Position", Position);
	}

	glm::vec3 TransformDirection = TransformComp->GetForward();
	if (TransformDirection != Direction)
	{
		Direction = TransformDirection;
		LShader::SetGlobalVec(SpotLightElement.str() + "Direction", Direction);
	}

	if (IsDirty)
	{
		LShader::SetGlobalVec(SpotLightElement.str() + "Ambient", Ambient);
		LShader::SetGlobalVec(SpotLightElement.str() + "Diffuse", Diffuse);
		LShader::SetGlobalVec(SpotLightElement.str() + "Specular", Specular);

		LShader::SetGlobalFloat(SpotLightElement.str() + "CutOff", CutOff);
		LShader::SetGlobalFloat(SpotLightElement.str() + "OuterCutOff", OuterCutOff);

		LShader::SetGlobalFloat(SpotLightElement.str() + "Constant", Constant);
		LShader::SetGlobalFloat(SpotLightElement.str() + "Linear", Linear);
		LShader::SetGlobalFloat(SpotLightElement.str() + "Quadratic", Quadratic);

		IsDirty = false;
	}
}

void CSpotLightComponent::UpdateSpotLights()
{
	if (IsCountDirty)
	{
		LShader::SetGlobalInt("ActiveSpotLights", SpotLightCount);
		IsCountDirty = false;
	}

	assert(SpotLightCount <= MAX_NUM_SPOT_LIGHT);
	for (unsigned int i = 0; i < SpotLightCount; i++)
	{
		CSpotLightComponent* SpotLightComp = LECS::Get()->GetComponent<CSpotLightComponent>(SpotLightEntityIDs[i]);
		if (SpotLightComp)
		{
			SpotLightComp->Update(i);
		}
	}
}
