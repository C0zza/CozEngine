#include "stdafx.h"

#include "RendererInfo.h"

#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
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
	CameraComponentSystem = nullptr;
}

void LRendererInfo::UpdateProjectionMatrix(const int Width, const int Height)
{
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 5000.f);
}

const glm::mat4& LRendererInfo::GetViewMatrix() const
{
	if (CameraComponentSystem)
	{
		return CameraComponentSystem->GetViewMatrix();
	}
}

const glm::vec3& LRendererInfo::GetViewPos()
{
	if (!CameraComponentSystem)
	{
		LECS* ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
		CameraComponentSystem = dynamic_cast<CCameraComponentSystem*>(ECS->GetComponentSystemFor<CCameraComponent>());
	}

	if (CameraComponentSystem)
	{
		return CameraComponentSystem->GetViewPos();
	}
}
