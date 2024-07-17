#include "PlayerEntity.h"

#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Game/Components/Movement.h"

CPlayerEntity::CPlayerEntity()
{
	CCameraComponent* CameraComponent = AddComponent<CCameraComponent>();
	if (CameraComponent)
	{
		CCameraComponentSystem* CameraComponentSys = nullptr;
		if (LECS* ECS = GetECS())
		{
			CameraComponentSys = dynamic_cast<CCameraComponentSystem*>(ECS->GetComponentSystemFor<CCameraComponent>());
		}

		if (CameraComponentSys)
		{
			CameraComponentSys->ActivateCamera(*CameraComponent);
		}
		else
		{
			Log(LLogLevel::WARNING, "CPlayerEntity::CPlayerEntity - Failed to activate camera");
		}
	}
	else
	{
		Log(LLogLevel::WARNING, "CPlayerEntity::CPlayerEntity - Failed to add camera component");
	}

	if (CTransformComponent* TransformComponent = GetComponent<CTransformComponent>())
	{
		TransformComponent->Move(glm::vec3(0.f, 0.f, 10.0f));
	}

	CMovement* Movement = AddComponent<CMovement>();
	Movement->Speed = 0.5f;
}