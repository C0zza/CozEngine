#include "stdafx.h"

#include "RendererInfo.h"

#include "CameraManager.h"
#include "ECS/ECS.h"
#include "Globes.h"
#include "Rendering/Renderer.h"

void LRendererInfo::Initialize()
{
	Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	// TODO: Ensure
}

void LRendererInfo::Deinitialize()
{
	Renderer = nullptr;
	CameraManager = nullptr;
}

void LRendererInfo::UpdateProjectionMatrix(const int Width, const int Height)
{
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 5000.f);
}

const glm::mat4& LRendererInfo::GetViewMatrix() const
{
	return CameraManager->GetViewMatrix();
}

const glm::vec3& LRendererInfo::GetViewPos()
{
	return CameraManager->GetViewPos();
}

bool LRendererInfo::CanRender()
{
	if (LCameraManager* Manager = GetCameraManager())
	{
		return CameraManager->GetActiveCameraEntityID() >= 0;
	}
	else
	{
		return false;
	}
}

LCameraManager* LRendererInfo::GetCameraManager()
{
	if (!CameraManager)
	{
		CameraManager = CSystem.GetSubsystems().GetSubsystem<LCameraManager>();
	}

	return CameraManager;
}
