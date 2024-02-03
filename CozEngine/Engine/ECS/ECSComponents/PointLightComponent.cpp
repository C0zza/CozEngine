#include "PointLightComponent.h"

#include <sstream>

#include "ECS/ECS.h"
#include "Globes.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

std::vector<LEntityID> CPointLightComponent::PointLights = {};
unsigned int CPointLightComponent::PointLightCount = 0;
bool CPointLightComponent::IsCountDirty = true;

CPointLightComponent::CPointLightComponent()
	: Constant{ 1.f }, Linear{ 0.f }, Quadratic{ 0.f }
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);
	Position = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

void CPointLightComponent::Init()
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

void CPointLightComponent::Destroy()
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

			assert(ECS);
			CPointLightComponent* PointLightComp = ECS->GetComponent<CPointLightComponent>(PointLights[Index]);
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

void CPointLightComponent::SetAmbient(const glm::vec3& i_Ambient)
{
	LLighting::AssertRGBVec(i_Ambient);
	SetDirtyMember(Ambient, i_Ambient);
}

void CPointLightComponent::SetDiffuse(const glm::vec3& i_Diffuse)
{
	LLighting::AssertRGBVec(i_Diffuse);
	SetDirtyMember(Diffuse, i_Diffuse);
}

void CPointLightComponent::SetSpecular(const glm::vec3& i_Specular)
{
	LLighting::AssertRGBVec(i_Specular);
	SetDirtyMember(Specular, i_Specular);
}

void CPointLightComponent::Update(const unsigned int Index)
{
	assert(Index >= 0 && Index <= CPointLightComponent::PointLightCount);

	assert(ECS);
	CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(EntityID);
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

void CPointLightComponent::UpdatePointLights()
{
	if (IsCountDirty)
	{
		LShader::SetGlobalInt("ActivePointLights", PointLightCount);
		IsCountDirty = false;
	}

	LECS* ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	assert(ECS);

	assert(PointLightCount <= MAX_NUM_POINT_LIGHT);
	for (unsigned int i = 0; i < PointLightCount; i++)
	{
		CPointLightComponent* PointLightComp = ECS->GetComponent<CPointLightComponent>(PointLights[i]);
		if (PointLightComp)
		{
			PointLightComp->Update(i);
		}
	}
}
