#pragma once

#include "Components/Component.h"

#include <glm/vec3.hpp>

#include "DirtyFlag.h"

class CDirectionalLightComponent : public CComponent, public LDirtyFlag
{
public:
	CDirectionalLightComponent();
	~CDirectionalLightComponent();

	void SetAmbient(const glm::vec3& i_Ambient) { SetDirtyMember(Ambient, i_Ambient); }
	void SetDiffuse(const glm::vec3& i_Diffuse) { SetDirtyMember(Diffuse, i_Diffuse); }
	void SetSpecular(const glm::vec3& i_Specular) { SetDirtyMember(Specular, i_Specular); }

	// TODO: Separate tickable and non-tickable components
	virtual void Tick() override {};

	void Update();

private:
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

