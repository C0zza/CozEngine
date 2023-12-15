#include "ECSPointLightComponent.h"

#include <sstream>

#include "ECS/ECS.h"
#include "ECSTransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

std::vector<LEntityID> CECSPointLightComponent::PointLights = {};
unsigned int CECSPointLightComponent::PointLightCount = 0;
bool CECSPointLightComponent::IsCountDirty = true;

CECSPointLightComponent::CECSPointLightComponent()
	: Constant{ 1.f }, Linear{ 0.f }, Quadratic{ 0.f }
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);
	Position = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

void CECSPointLightComponent::Init()
{
	if (PointLightCount < MAX_NUM_POINT_LIGHT)
	{
		PointLightCount++;
		IsCountDirty = true;
	}

	PointLights.push_back(EntityID);

	if (PointLights.size() <= MAX_NUM_POINT_LIGHT)
	{
		std::stringstream PointLightElement;
		PointLightElement << "PointLights[" << PointLights.size() - 1 << "].";
		LShader::SetGlobalVec(PointLightElement.str() + "Position", Position);
	}
}

void CECSPointLightComponent::Destroy()
{
	std::vector<LEntityID>::iterator it = std::find(PointLights.begin(), PointLights.end(), EntityID);

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

			CECSPointLightComponent* PointLightComp = LECS::Get()->GetComponent<CECSPointLightComponent>(PointLights[Index]);
			assert(PointLightComp);
			PointLightComp->IsDirty = true;

			if (PointLights.size() <= MAX_NUM_POINT_LIGHT)
			{
				PointLightCount--;
				IsCountDirty = true;
			}

			PointLights.pop_back();
		}
	}
}

void CECSPointLightComponent::SetAmbient(const glm::vec3& i_Ambient)
{
	LLighting::AssertRGBVec(i_Ambient);
	SetDirtyMember(Ambient, i_Ambient);
}

void CECSPointLightComponent::SetDiffuse(const glm::vec3& i_Diffuse)
{
	LLighting::AssertRGBVec(i_Diffuse);
	SetDirtyMember(Diffuse, i_Diffuse);
}

void CECSPointLightComponent::SetSpecular(const glm::vec3& i_Specular)
{
	LLighting::AssertRGBVec(i_Specular);
	SetDirtyMember(Specular, i_Specular);
}

void CECSPointLightComponent::Update(const unsigned int Index)
{
	assert(Index >= 0 && Index <= CECSPointLightComponent::PointLightCount);

	CECSTransformComponent* TransformComp = LECS::Get()->GetComponent<CECSTransformComponent>(EntityID);
	assert(TransformComp);

	std::stringstream PointLightElement;
	PointLightElement << "PointLights[" << Index << "]";

	if (TransformComp->GetPosition() != Position)
	{
		Position = TransformComp->GetPosition();
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

void CECSPointLightComponent::UpdatePointLights()
{
	if (IsCountDirty)
	{
		LShader::SetGlobalInt("ActivePointLights", PointLightCount);
		IsCountDirty = false;
	}

	assert(PointLightCount <= MAX_NUM_POINT_LIGHT);
	for (unsigned int i = 0; i < PointLightCount; i++)
	{
		CECSPointLightComponent* PointLightComp = LECS::Get()->GetComponent<CECSPointLightComponent>(PointLights[i]);
		if (PointLightComp)
		{
			PointLightComp->Update(i);
		}
	}
}
