#include "PlayerEntity.h"

#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/ECSCameraComponent.h"
#include "ECS/ECSComponents/ECSTransformComponent.h"
#include "Game/Components/Movement.h"

CPlayerEntity::CPlayerEntity()
{
	AddComponent<CECSCameraComponent>();
	if (CECSCameraComponent* CameraComponent = GetComponent<CECSCameraComponent>())
	{
		CameraComponent->ActivateCamera();
	}

	if (CECSTransformComponent* TransformComponent = GetComponent<CECSTransformComponent>())
	{
		TransformComponent->Move(glm::vec3(0.f, 0.f, 10.0f));
	}

	AddComponent<CMovement>();
}