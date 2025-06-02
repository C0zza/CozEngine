#include "stdafx.h"

#include "MathUtility.h"

glm::vec2 LMathUtility::WindowToTextureCoords(const glm::vec2& WindowMouseCoords, const glm::vec2& TextureTopLeftCoords, const glm::vec2& TextureBottomRightCoords)
{
	const float XPos = WindowMouseCoords.x - TextureTopLeftCoords.x;
	const float YPos = TextureBottomRightCoords.y - TextureTopLeftCoords.y - (WindowMouseCoords.y - TextureTopLeftCoords.y);
	return glm::vec2(XPos, YPos);
}

ImVec2 LMathUtility::WindowToTextureCoords(const ImVec2& WindowMouseCoords, const ImVec2& TextureTopLeftCoords, const ImVec2& TextureBottomRightCoords)
{
	const float XPos = WindowMouseCoords.x - TextureTopLeftCoords.x;
	const float YPos = TextureBottomRightCoords.y - TextureTopLeftCoords.y - (WindowMouseCoords.y - TextureTopLeftCoords.y);
	return ImVec2(XPos, YPos);
}
