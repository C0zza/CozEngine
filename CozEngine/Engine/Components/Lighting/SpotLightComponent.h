#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "Components/Component.h"
#include "DirtyFlag.h"

// Should match number in shader.fs
#define MAX_NUM_SPOT_LIGHT 4

class CSpotLightComponent : public CComponent, public LDirtyFlag
{
public:
	CSpotLightComponent();
	~CSpotLightComponent();

	void SetAmbient(const glm::vec3& i_Ambient) { SetDirtyMember(Ambient, i_Ambient); }
	void SetDiffuse(const glm::vec3& i_Diffuse) { SetDirtyMember(Diffuse, i_Diffuse); }
	void SetSpecular(const glm::vec3& i_Specular) { SetDirtyMember(Specular, i_Specular); }

	void SetCutOff(const float i_CutOff) { SetDirtyMember(CutOff, i_CutOff); }
	void SetOuterCutOff(const float i_OuterCutOff) { SetDirtyMember(OuterCutOff, i_OuterCutOff); }

	// TODO: Separate tickable and non-tickable components
	virtual void Tick() override {};

private:
	void Update(const int Index);

	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float CutOff;
	float OuterCutOff;

public:
	static void UpdateSpotLights();

private:
	static std::vector<CSpotLightComponent*> SpotLights;
	
	static unsigned int SpotLightCount;
	static bool IsCountDirty;
};
