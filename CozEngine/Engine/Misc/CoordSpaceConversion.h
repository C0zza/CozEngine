#pragma once

#include <glm/vec2.hpp>

class LCoordSpaceConversion
{
public:
	// Assumes ScreenCoords is local to the relevant "screen". E.g. The scene viewport in editor
	static glm::vec2 ScreenToUV(const glm::vec2& ScreenCoords, const float Width, const float Height);
	static glm::vec2 ScreenToClip(const glm::vec2& ScreenCoords, const float Width, const float Height);
	static glm::vec3 ScreenToWorldRay(const glm::vec2& ScreenCoords, const float Width, const float Height);
};

