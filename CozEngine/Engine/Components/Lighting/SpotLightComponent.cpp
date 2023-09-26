#include "SpotLightComponent.h"

#include <sstream>

#include "Object.h"
#include "Rendering/Shader.h"

std::vector<CSpotLightComponent*> CSpotLightComponent::SpotLights{};
unsigned int CSpotLightComponent::SpotLightCount = 0;
bool CSpotLightComponent::IsCountDirty = true;

CSpotLightComponent::CSpotLightComponent()
	: CutOff{ 0.f }, OuterCutOff{ 0.f }, Constant{ 0.f }, Linear{ 0.f }, Quadratic{ 0.f }
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

	SpotLights.push_back(this);
	
	if (SpotLights.size() <= MAX_NUM_SPOT_LIGHT)
	{
		std::stringstream SpotLightElement;
		SpotLightElement << "SpotLights[" << SpotLights.size() - 1 << "].";
		LShader::SetGlobalVec(SpotLightElement.str() + "Position", Position);
		LShader::SetGlobalVec(SpotLightElement.str() + "Direction", Direction);
	}
}

CSpotLightComponent::~CSpotLightComponent()
{
	std::vector<CSpotLightComponent*>::iterator it = std::find(SpotLights.begin(), SpotLights.end(), this);

	assert(it != SpotLights.end());

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
			SpotLightCount--;
			IsCountDirty = true;
		}
		else
		{
			SpotLights[Index] = SpotLights[SpotLights.size() - 1];
			SpotLights[Index]->IsDirty = true;

			if (SpotLights.size() <= MAX_NUM_SPOT_LIGHT)
			{
				SpotLightCount--;
				IsCountDirty = true;
			}

			SpotLights.pop_back();
		}
	}
}

void CSpotLightComponent::Update(const int Index)
{
	assert(Index >= 0 && Index <= CSpotLightComponent::SpotLightCount);
	assert(Parent);

	std::stringstream SpotLightElement;
	SpotLightElement << "SpotLights[" << Index << "].";

	if (Parent->Transform.GetPosition() != Position)
	{
		Position = Parent->Transform.GetPosition();
		LShader::SetGlobalVec(SpotLightElement.str() + "Position", Position);
	}

	glm::vec3 TransformDirection = Parent->Transform.GetForward();
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
		SpotLights[i]->Update(i);
	}
}
