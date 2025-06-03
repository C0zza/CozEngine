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

glm::vec3 LCoordSpaceConversion::ScreenToWorldRay(const glm::vec2& ScreenCoords, const float Width, const float Height)
{
	LRendererInfo* RendererInfo = CSystem.GetSubsystems().GetSubsystem<LRendererInfo>();

	const glm::vec2 ClipSpace2D = ScreenToClip(ScreenCoords, Width, Height);

	// OpenGL uses -1 for near clip space
	const glm::vec4 ClipSpace(ClipSpace2D.x, ClipSpace2D.y, -1, 1);

	const glm::mat4 InverseProjection = glm::inverse(RendererInfo->GetProjectionMatrix());

	glm::vec4 ViewSpace = InverseProjection * ClipSpace;

	const glm::mat4 InverseView = glm::inverse(RendererInfo->GetViewMatrix());

	glm::vec4 WorldSpace = InverseView * ViewSpace;

	glm::vec3 RayDirection = glm::vec3(WorldSpace.x, WorldSpace.y, WorldSpace.z) - RendererInfo->GetViewPos();

	return RendererInfo->GetViewPos() + glm::normalize(RayDirection);
}
