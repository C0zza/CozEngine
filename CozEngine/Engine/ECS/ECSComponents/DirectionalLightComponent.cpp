#include "DirectionalLightComponent.h"

#include <cassert>
#include <sstream>

#include "ECS/ECS.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

CDirectionalLightComponent* CDirectionalLightComponent::ActiveDirectionalLight = nullptr;

CDirectionalLightComponent::CDirectionalLightComponent()
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);
	Direction = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

void CDirectionalLightComponent::Init()
{
	assert(!ActiveDirectionalLight);
	ActiveDirectionalLight = this;

	LShader::SetGlobalBool("DirectionalLight.IsActive", true);
	LShader::SetGlobalVec("DirectionalLight.Direction", Direction);
}

void CDirectionalLightComponent::Destroy()
{
	assert(ActiveDirectionalLight);
	if (ActiveDirectionalLight == this)
	{
		ActiveDirectionalLight = nullptr;
		LShader::SetGlobalBool("DirectionalLight.IsActive", false);
	}
}

void CDirectionalLightComponent::SetAmbient(const glm::vec3& i_Ambient)
{
	LLighting::AssertRGBVec(i_Ambient);
	SetDirtyMember(Ambient, i_Ambient);
}

void CDirectionalLightComponent::SetDiffuse(const glm::vec3& i_Diffuse)
{
	LLighting::AssertRGBVec(i_Diffuse);
	SetDirtyMember(Diffuse, i_Diffuse);
}

void CDirectionalLightComponent::SetSpecular(const glm::vec3& i_Specular)
{
	LLighting::AssertRGBVec(i_Specular);
	SetDirtyMember(Specular, i_Specular);
}

void CDirectionalLightComponent::Update()
{
	assert(ECS);
	CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(EntityID);
	assert(TransformComp);

	std::stringstream DirectionalLightVar;
	DirectionalLightVar << "DirectionalLight.";

	if (TransformComp->GetForward() != Direction)
	{
		Direction = TransformComp->GetForward();
		LShader::SetGlobalVec(DirectionalLightVar.str() + "Direction", Direction);
	}

	if (IsDirty)
	{
		LShader::SetGlobalVec(DirectionalLightVar.str() + "Ambient", Ambient);
		LShader::SetGlobalVec(DirectionalLightVar.str() + "Diffuse", Diffuse);
		LShader::SetGlobalVec(DirectionalLightVar.str() + "Specular", Specular);
		IsDirty = false;
	}
}

void CDirectionalLightComponent::UpdateDirectionalLight()
{
	if (ActiveDirectionalLight)
	{
		ActiveDirectionalLight->Update();
	}
}
