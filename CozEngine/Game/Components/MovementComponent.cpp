#include "MovementComponent.h"

#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Misc/GlmSerialization.h"

void CMovementSystem::GetSerializedComponent(const CMovementComponent& Component, nlohmann::json& J) const
{
	J["Movement"] = Component.Movement;
}

void CMovementSystem::DeserializeComponent(CMovementComponent& Component, const nlohmann::json& J)
{
	Component.Movement = J["Movement"];
}

void CMovementSystem::RunComponent(CMovementComponent& Component)
{
	const bool bUpdateMovement = glm::length(Component.Movement) > 0;
	const bool bUpdateRotation = glm::length(Component.Rotation) > 0;

	CTransformComponent* TransformComponent = nullptr;
	if (bUpdateMovement || bUpdateRotation)
	{
		TransformComponent = ECS->GetComponent<CTransformComponent>(Component.EntityID);
		if (!TransformComponent)
		{
			return;
		}
	
		if (bUpdateMovement)
		{
			TransformComponent->MoveRelative(glm::normalize(Component.Movement) * Component.Speed);
		}

		if (bUpdateRotation)
		{
			TransformComponent->Rotate(Component.Rotation * Component.Sensitivity);
		}
	}
}
