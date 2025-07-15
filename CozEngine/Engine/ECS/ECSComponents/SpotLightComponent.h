#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

#include "Reflection/Reflection.h"

// Should match number in shader.fs
#define MAX_NUM_SPOT_LIGHT 4

REFL_CLASS()
struct CSpotLightComponent : public LECSComponent
{
	REFL_GENERATED_BODY(CSpotLightComponent)
public:
	CSpotLightComponent();

	REFL_PROP(Visible)
	glm::vec3 Position;
	REFL_PROP(Visible)
	glm::vec3 Direction;
	REFL_PROP(Visible)
	glm::vec3 Ambient;
	REFL_PROP(Visible)
	glm::vec3 Diffuse;
	REFL_PROP(Visible)
	glm::vec3 Specular;

	REFL_PROP(Visible)
	float CutOff;
	REFL_PROP(Visible)
	float OuterCutOff;

	REFL_PROP(Visible)
	float Constant;
	REFL_PROP(Visible)
	float Linear;
	REFL_PROP(Visible)
	float Quadratic;
};

//class CSpotLightComponentSystem : public LComponentSystem<CSpotLightComponent>
//{
//public:
//	void UpdateSpotLights();
//
//	virtual const char* GetComponentName() const final { return "SpotLightComponentSystem"; }
//
//#if defined(COZ_EDITOR)
//	virtual void DrawImGuiComponent(CSpotLightComponent& Component) final;
//#endif
//
//protected:
//	virtual void OnComponentAdded(CSpotLightComponent& Component) override;
//	virtual void OnComponentRemoved(CSpotLightComponent& Component) override;
//
//	virtual void GetSerializedComponent(const CSpotLightComponent& Component, nlohmann::json& J) const final;
//	virtual void DeserializeComponent(CSpotLightComponent& Component, const nlohmann::json& J) final;
//
//private:
//	void UpdateSpotLight(CSpotLightComponent* Component, int Index);
//
//	std::vector<LEntityID> SpotLights;
//
//	CSpotLightComponent SpotLightCache[MAX_NUM_SPOT_LIGHT];
//
//	int SpotLightCount = 0;
//	bool IsCountDirty = true;
//
//};

