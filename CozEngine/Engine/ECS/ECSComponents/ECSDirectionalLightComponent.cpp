#include "ECSDirectionalLightComponent.h"

#include <cassert>
#include <sstream>

#include "ECS/ECS.h"
#include "ECSTransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Shader.h"

CECSDirectionalLightComponent* CECSDirectionalLightComponent::ActiveDirectionalLight = nullptr;

CECSDirectionalLightComponent::CECSDirectionalLightComponent()
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);
	Direction = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

void CECSDirectionalLightComponent::Init()
{
	assert(!ActiveDirectionalLight);
	ActiveDirectionalLight = this;

	LShader::SetGlobalBool("DirectionalLight.IsActive", true);
	LShader::SetGlobalVec("DirectionalLight.Direction", Direction);
}

void CECSDirectionalLightComponent::Destroy()
{
	assert(ActiveDirectionalLight);
	if (ActiveDirectionalLight == this)
	{
		ActiveDirectionalLight = nullptr;
		LShader::SetGlobalBool("DirectionalLight.IsActive", false);
	}
}

void CECSDirectionalLightComponent::SetAmbient(const glm::vec3& i_Ambient)
{
	LLighting::AssertRGBVec(i_Ambient);
	SetDirtyMember(Ambient, i_Ambient);
}

void CECSDirectionalLightComponent::SetDiffuse(const glm::vec3& i_Diffuse)
{
	LLighting::AssertRGBVec(i_Diffuse);
	SetDirtyMember(Diffuse, i_Diffuse);
}

void CECSDirectionalLightComponent::SetSpecular(const glm::vec3& i_Specular)
{
	LLighting::AssertRGBVec(i_Specular);
	SetDirtyMember(Specular, i_Specular);
}

void CECSDirectionalLightComponent::Update()
{
	CECSTransformComponent* TransformComp = LECS::Get()->GetComponent<CECSTransformComponent>(EntityID);
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

void CECSDirectionalLightComponent::UpdateDirectionalLight()
{
	if (ActiveDirectionalLight)
	{
		ActiveDirectionalLight->Update();
	}
}
