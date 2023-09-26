#include "PointLightComponent.h"

#include <sstream>

#include "Object.h"
#include "Rendering/Shader.h"

std::vector<CPointLightComponent*> CPointLightComponent::PointLights = {};
unsigned int CPointLightComponent::PointLightCount = 0;
bool CPointLightComponent::IsCountDirty = true;

CPointLightComponent::CPointLightComponent()
	: Constant{ 1.f }, Linear{ 0.f }, Quadratic{ 0.f }, IsDirty{ true }
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);

	Position = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;

	if (PointLightCount < MAX_NUM_POINT_LIGHT)
	{
		PointLightCount++;
		IsCountDirty = true;
	}

	PointLights.push_back(this);

	if (PointLights.size() <= MAX_NUM_POINT_LIGHT)
	{
		std::stringstream PointLightElement;
		PointLightElement << "PointLights[" << PointLights.size() - 1 << "].";
		LShader::SetGlobalVec(PointLightElement.str() + "Position", Position);
	}
}

CPointLightComponent::~CPointLightComponent()
{
	std::vector<CPointLightComponent*>::iterator it = std::find(PointLights.begin(), PointLights.end(), this);

	assert(it != PointLights.end());

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
			PointLightCount--;
			IsCountDirty = true;
		}
		else
		{
			PointLights[Index] = PointLights[PointLights.size() - 1];
			PointLights[Index]->IsDirty = true;

			if (PointLights.size() <= MAX_NUM_POINT_LIGHT)
			{
				PointLightCount--;
				IsCountDirty = true;
			}

			PointLights.pop_back();
		}
	}
}

void CPointLightComponent::Update(const int Index)
{
	assert(Index >= 0 && Index <= CPointLightComponent::PointLightCount);
	assert(Parent);

	std::stringstream PointLightElement;
	PointLightElement << "PointLights[" << Index << "]";

	if (Parent->Transform.GetPosition() != Position)
	{
		Position = Parent->Transform.GetPosition();
		LShader::SetGlobalVec(PointLightElement.str() + ".Position", Position);
	}

	if (IsDirty)
	{
		LShader::SetGlobalVec(PointLightElement.str() + ".Ambient", Ambient);
		LShader::SetGlobalVec(PointLightElement.str() + ".Diffuse", Diffuse);
		LShader::SetGlobalVec(PointLightElement.str() + ".Specular", Specular);
		LShader::SetGlobalFloat(PointLightElement.str() + ".Constant", Constant);
		LShader::SetGlobalFloat(PointLightElement.str() + ".Linear", Linear);
		LShader::SetGlobalFloat(PointLightElement.str() + ".Quadratic", Quadratic);
		IsDirty = false;
	}
}

void CPointLightComponent::UpdatePointLights()
{
	if (IsCountDirty)
	{
		LShader::SetGlobalInt("ActivePointLights", PointLightCount);
		IsCountDirty = false;
	}

	assert(PointLightCount <= MAX_NUM_POINT_LIGHT);
	for (unsigned int i = 0; i < PointLightCount; i++)
	{
		PointLights[i]->Update(i);
	}
}
