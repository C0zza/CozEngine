#include "stdafx.h"

#include "CoordSpaceConversion.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Globes.h"
#include "Rendering/RendererInfo.h"

glm::vec2 LCoordSpaceConversion::ScreenToUV(const glm::vec2& ScreenCoords, const float Width, const float Height)
{
	return glm::vec2(ScreenCoords.x, Height - ScreenCoords.y);
}

glm::vec2 LCoordSpaceConversion::ScreenToClip(const glm::vec2& ScreenCoords, const float Width, const float Height)
{
	return glm::vec2((ScreenCoords.x / Width) * 2 - 1, 1 - (ScreenCoords.y / Height) * 2);
}

glm::vec3 LCoordSpaceConversion::ScreenToWorldPos(const glm::vec2& ScreenCoords, const float Width, const float Height, const float Distance)
{
	LRendererInfo* RendererInfo = CSystem.GetSubsystems().GetSubsystem<LRendererInfo>();
	assert(RendererInfo);

	// OpenGL uses -1 for near clip space
	const glm::vec4 ClipSpace(ScreenToClip(ScreenCoords, Width, Height), -1, 1);

	const glm::vec4 WorldSpace = glm::inverse(RendererInfo->GetProjectionMatrix() * RendererInfo->GetViewMatrix()) * ClipSpace;

	glm::vec3 RayDirection = glm::vec3(WorldSpace.x, WorldSpace.y, WorldSpace.z) / WorldSpace.w - RendererInfo->GetViewPos();

	return RendererInfo->GetViewPos() + glm::normalize(RayDirection) * Distance;
}
