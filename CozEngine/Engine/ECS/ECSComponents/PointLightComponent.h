#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "Misc/DirtyFlag.h"
#include "ECS/ECSComponents/ECSComponent.h"

// Should match number in shader.fs
#define MAX_NUM_POINT_LIGHT 4

struct CPointLightComponent : public LECSComponent, public LDirtyFlag
{
	CPointLightComponent();

	virtual void Init() override;
	virtual void Destroy() override;

	void SetAmbient(const glm::vec3& i_Ambient);
	void SetDiffuse(const glm::vec3& i_Diffuse);
	void SetSpecular(const glm::vec3& i_Specular);

	// See https://learnopengl.com/Lighting/Light-casters for general point light values
	void SetConstant(const float i_Constant) { SetDirtyMember(Constant, i_Constant); };
	void SetLinear(const float i_Linear) { SetDirtyMember(Linear, i_Linear); }
	void SetQuadratic(const float i_Quadratic) { SetDirtyMember(Quadratic, i_Quadratic); }

private:
	void Update(const unsigned int Index);

	glm::vec3 Position;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;

public:
	static void UpdatePointLights();

private:
	static std::vector<LEntityID> PointLights;

	static unsigned int PointLightCount;
	static bool IsCountDirty;
};

