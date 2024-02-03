#pragma once

#include <glm/vec3.hpp>

#include "Misc/DirtyFlag.h"
#include "ECS/ECSComponents/ECSComponent.h"

struct CDirectionalLightComponent : public LECSComponent, public LDirtyFlag
{
public:
	CDirectionalLightComponent();

	virtual void Destroy() override;

	void SetAmbient(const glm::vec3& i_Ambient);
	void SetDiffuse(const glm::vec3& i_Diffuse);
	void SetSpecular(const glm::vec3& i_Specular);

protected:
	virtual void Init() override;

private:
	void Update();

	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

public:
	static void UpdateDirectionalLight();

private:
	// We assume 1 directional light existing at any 1 time. Could be better as simply a member of something?
	static CDirectionalLightComponent* ActiveDirectionalLight;
};

