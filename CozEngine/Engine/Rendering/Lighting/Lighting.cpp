#include "Lighting.h"

#include <glm/vec3.hpp>

#include "Rendering/Shader.h"

namespace LLighting
{
	void UpdateLightComponentVec(glm::vec3& CachedVec, const glm::vec3& NewVec, const char* ShaderVarString, const std::stringstream& ShaderString)
	{
		if (CachedVec != NewVec)
		{
			LShader::SetGlobalVec(ShaderString.str() + ShaderVarString, NewVec);
			CachedVec = NewVec;
		}
	}

	void UpdateLightComponentFloat(float& CachedFloat, const float& NewFloat, const char* ShaderVarString, const std::stringstream& ShaderString)
	{
		if (CachedFloat != NewFloat)
		{
			LShader::SetGlobalFloat(ShaderString.str() + ShaderVarString, NewFloat);
			CachedFloat = NewFloat;
		}
	}

	void AssertRGBVec(const glm::vec3& Vec)
	{
		assert(Vec.x >= 0.f && Vec.x <= 1.f);
		assert(Vec.y >= 0.f && Vec.y <= 1.f);
		assert(Vec.z >= 0.f && Vec.z <= 1.f);
	}
}
