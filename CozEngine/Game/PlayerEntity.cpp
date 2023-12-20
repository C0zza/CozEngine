#include "PlayerEntity.h"

#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Game/Components/Movement.h"

CPlayerEntity::CPlayerEntity()
{
	AddComponent<CCameraComponent>();
	if (CCameraComponent* CameraComponent = GetComponent<CCameraComponent>())
	{
		CameraComponent->ActivateCamera();
	}

	if (CTransformComponent* TransformComponent = GetComponent<CTransformComponent>())
	{
		TransformComponent->Move(glm::vec3(0.f, 0.f, 10.0f));
	}

	AddComponent<CMovement>();
}