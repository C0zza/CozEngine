#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "Components/Component.h"

// Should match number in shader.fs
#define MAX_NUM_POINT_LIGHT 4

class CPointLightComponent : public CComponent
{
public:
	CPointLightComponent();
	~CPointLightComponent();

	void SetAmbient(const glm::vec3& i_Ambient) { UpdateMember(Ambient, i_Ambient); }
	void SetDiffuse(const glm::vec3& i_Diffuse) { UpdateMember(Diffuse, i_Diffuse); }
	void SetSpecular(const glm::vec3& i_Specular) { UpdateMember(Specular, i_Specular); }

	// See https://learnopengl.com/Lighting/Light-casters for general point light values
	void SetConstant(const float i_Constant) { UpdateMember(Quadratic, i_Constant); }
	void SetLinear(const float i_Linear) { UpdateMember(Quadratic, i_Linear); }
	void SetQuadratic(const float i_Quadratic) { UpdateMember(Quadratic, i_Quadratic); }

	// TODO: Separate tickable and non-tickable components
	virtual void Tick() override {}

private:
	void Update(const int Index);

	template<typename T>
	void UpdateMember(T& Member, const T Update)
	{
		if (Member != Update)
		{
			Member = Update;
			IsDirty = true;
		}
	}

	glm::vec3 Position;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;

	bool IsDirty;

public:
	static void UpdatePointLights();

private:
	static std::vector<CPointLightComponent*> PointLights;

	static unsigned int PointLightCount;
	static bool IsCountDirty;
};

