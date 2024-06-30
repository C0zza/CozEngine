#pragma once

#include <glm/fwd.hpp>

namespace LLighting
{
	void UpdateLightComponentVec(glm::vec3& CachedVec, const glm::vec3& NewVec, const char* ShaderVarString, const std::stringstream& ShaderString);
	void UpdateLightComponentFloat(float& CachedFloat, const float& NewFloat, const char* ShaderVarString, const std::stringstream& ShaderString);

	void AssertRGBVec(const glm::vec3& vec);
}

