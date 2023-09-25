#include "DirectionalLightComponent.h"

#include <sstream>

#include "Object.h"
#include "Rendering/Shader.h"

CDirectionalLightComponent* CDirectionalLightComponent::ActiveDirectionalLight = nullptr;

CDirectionalLightComponent::CDirectionalLightComponent()
{
	// Only 1 directional light should exist at any 1 time.
	assert(!ActiveDirectionalLight);
	ActiveDirectionalLight = this;
	LShader::SetGlobalBool("DirectionalLight.IsActive", true);

	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);

	Direction = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

CDirectionalLightComponent::~CDirectionalLightComponent()
{
	assert(ActiveDirectionalLight);
	if (ActiveDirectionalLight == this)
	{
		ActiveDirectionalLight = nullptr;
		LShader::SetGlobalBool("DirectionalLight.IsActive", false);
	}
}

void CDirectionalLightComponent::Update()
{
	assert(Parent);

	std::stringstream DirectionalLightVar;
	DirectionalLightVar << "DirectionalLight.";

	if (Parent->Transform.GetForward() != Direction)
	{
		Direction = Parent->Transform.GetForward();
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
