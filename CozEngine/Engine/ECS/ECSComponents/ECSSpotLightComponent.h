#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "ECS/ComponentSystem.h"
#include "DirtyFlag.h"
#include "ECS/ECSComponents/ECSComponent.h"

// Should match number in shader.fs
#define MAX_NUM_SPOT_LIGHT 4

struct CECSSpotLightComponent : public LECSComponent, LDirtyFlag
{
public:
	CECSSpotLightComponent();

	virtual void Init() override;
	virtual void Destroy() override;

	void SetAmbient(const glm::vec3& i_Ambient);
	void SetDiffuse(const glm::vec3& i_Diffuse);
	void SetSpecular(const glm::vec3& i_Specular);

	void SetCutOff(const float i_CutOff) { SetDirtyMember(CutOff, i_CutOff); }
	void SetOuterCutOff(const float i_OuterCutOff) { SetDirtyMember(OuterCutOff, i_OuterCutOff); }

	void SetConstant(const float i_Constant) { SetDirtyMember(Constant, i_Constant); }
	void SetLinear(const float i_Linear) { SetDirtyMember(Linear, i_Linear); }
	void SetQuadratic(const float i_Quadratic) { SetDirtyMember(Quadratic, i_Quadratic); }

private:
	void Update(const unsigned int Index);

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

public:
	static void UpdateSpotLights();

private:
	static std::vector<LEntityID> SpotLightEntityIDs;

	static unsigned int SpotLightCount;
	static bool IsCountDirty;
};

