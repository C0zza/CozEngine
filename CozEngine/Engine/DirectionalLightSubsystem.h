#pragma once

#include <glm/vec3.hpp>

#include "Subsystem.h"

#define MAX_NUM_SPOT_LIGHTS 4
#define MAX_NUM_POINT_LIGHTS 4

class LArchetypeManager;
class LRenderer;

struct CPointLightComponent;
struct CSpotLightComponent;
struct CTransformComponent;

class LDirectionalLightSubsystem : public LSubsystem
{
public:
	virtual void Initialize();

	void SetDirectionalLightEntityID(const LEntityID EntityID) { ActiveDirectionalLight = EntityID; }
	void SetSpotLightEntityIDs(std::vector<LEntityID> EntityIDs);
	void SetPointLightEntityIDs(std::vector<LEntityID> EntityIDs);

	void UpdateDirectionalLightData();
	void UpdateSpotLightData();
	void UpdatePointLightData();

private:
	void UpdateSpotLight(CSpotLightComponent* Component, CTransformComponent* Transform, int Index);
	void UpdatePointLight(CPointLightComponent* PointLight, CTransformComponent* Transform, int Index);

	void SetUBOData(const glm::vec3& Direction,
					const glm::vec3& Ambient,
					const glm::vec3& Diffuse, 
					const glm::vec3& Specular);

	void SetInvalidUBOData();

	LEntityID ActiveDirectionalLight = 0;

	LEntityID SpotLightIDs[MAX_NUM_SPOT_LIGHTS];
	int		  SpotLightCount = 0;

	LEntityID PointLightIDs[MAX_NUM_POINT_LIGHTS];
	int		  PointLightCount = 0;

private:
	LArchetypeManager* ArchetypeManager = nullptr;
	LRenderer* Renderer = nullptr;
};

