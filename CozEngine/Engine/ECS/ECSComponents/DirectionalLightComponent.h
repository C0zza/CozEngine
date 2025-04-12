#pragma once

#include <glm/vec3.hpp>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

struct CDirectionalLightComponent : public LECSComponent
{
	CDirectionalLightComponent();

	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
};

class CDirectionalLightComponentSystem : public LComponentSystem<CDirectionalLightComponent>
{
public:
	void UpdateDirectionalLight();

	virtual const char* GetComponentName() const final { return "DirectionalLightComponentSystem"; }

#if defined(COZ_EDITOR)
	virtual void DrawImGuiComponent(CDirectionalLightComponent& Component) final;
#endif

protected:
	virtual void OnComponentAdded(CDirectionalLightComponent& Component) override;
	virtual void OnComponentRemoved(CDirectionalLightComponent& Component) override;

	virtual void GetSerializedComponent(const CDirectionalLightComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CDirectionalLightComponent& Component, const nlohmann::json& J) final;

private:
	// We assume 1 directional light existing at any 1 time. Could be better as simply a member of something?
	CDirectionalLightComponent* ActiveDirectionalLight = nullptr;

	glm::vec3 CurrentAmbient;
	glm::vec3 CurrentDiffuse;
	glm::vec3 CurrentSpecular;
};

