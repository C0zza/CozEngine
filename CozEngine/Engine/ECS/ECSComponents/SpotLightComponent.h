#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

// Should match number in shader.fs
#define MAX_NUM_SPOT_LIGHT 4

struct CSpotLightComponent : public LECSComponent
{
public:
	CSpotLightComponent();

	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float CutOff;
	float OuterCutOff;

	float Constant;
	float Linear;
	float Quadratic;
};

class CSpotLightComponentSystem : public LComponentSystem<CSpotLightComponent>
{
public:
	void UpdateSpotLights();

	virtual const char* GetComponentName() const final { return "SpotLightComponentSystem"; }

protected:
	virtual void OnComponentAdded(CSpotLightComponent& Component) override;
	virtual void OnComponentRemoved(CSpotLightComponent& Component) override;

	virtual void GetSerializedComponent(const CSpotLightComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CSpotLightComponent& Component, const nlohmann::json& J) final;

private:
	void UpdateSpotLight(CSpotLightComponent* Component, int Index);

	std::vector<LEntityID> SpotLights;

	CSpotLightComponent SpotLightCache[MAX_NUM_SPOT_LIGHT];

	int SpotLightCount = 0;
	bool IsCountDirty = true;

};

