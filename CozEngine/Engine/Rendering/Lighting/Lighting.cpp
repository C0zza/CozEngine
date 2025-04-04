#include "Lighting.h"

#include <glm/vec3.hpp>

#include "Rendering/Shader.h"

namespace LLighting
{
	void AssertRGBVec(const glm::vec3& Vec)
	{
		assert(Vec.x >= 0.f && Vec.x <= 1.f);
		assert(Vec.y >= 0.f && Vec.y <= 1.f);
		assert(Vec.z >= 0.f && Vec.z <= 1.f);
	}
}
