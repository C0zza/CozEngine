#pragma once

#include <glm/vec2.hpp>

class LMathUtility
{
public:
	// Assumes texture fills
	static glm::vec2 WindowToTextureCoords(const glm::vec2&, const glm::vec2& TextureTopLeftCoords, const glm::vec2& TextureBottomRightCoords);
	static ImVec2 WindowToTextureCoords(const ImVec2&, const ImVec2& TextureTopLeftCoords, const ImVec2& TextureBottomRightCoords);
};

