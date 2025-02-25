#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Rendering/Lighting/Lighting.h"

// Should match number in shader.fs
#define MAX_NUM_POINT_LIGHT 4

struct CPointLightComponent : public LECSComponent
{
	CPointLightComponent();

	glm::vec3 Position;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	// See https://learnopengl.com/Lighting/Light-casters for general point light values
	float Constant;
	float Linear;
	float Quadratic;
};

class CPointLightComponentSystem : public LComponentSystem<CPointLightComponent>
{
public:
	void UpdatePointLights();

	virtual const char* GetComponentName() const final { return "PointLightComponentSystem"; }

protected:
	virtual void OnComponentAdded(CPointLightComponent& Component) override;
	virtual void OnComponentRemoved(CPointLightComponent& Component) override;

	virtual void GetSerializedComponent(const CPointLightComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CPointLightComponent& Component, const nlohmann::json& J) final;

private:
	void UpdatePointLight(CPointLightComponent* Component, int Index);

	std::vector<LEntityID> PointLights{};

	CPointLightComponent PointLightCache[MAX_NUM_POINT_LIGHT];

	int PointLightCount = 0;
	bool IsCountDirty = true;
};

