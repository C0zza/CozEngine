#include "Movement.h"

#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"

CMovement::CMovement()
{
	// TODO: DANGER DANGER, high chance of initialising something here and then since the ECS currently copies components into the vector 
	// this will cause bad stuff. Should have initialisation in init methods
}

void CMovement::Init()
{
	LInputManager* InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>(true);
	if (!InputManager)
	{
		Log(LLogLevel::WARNING, "CMovement::Init - Failed to init component after invalid LinputManager.");
		return;
	}

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_A, GLFW_PRESS), &CMovement::MoveLeft, MoveLeftEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_A, GLFW_RELEASE), &CMovement::StopMoveLeft, StopMoveLeftEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_D, GLFW_PRESS), &CMovement::MoveRight, MoveRightEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_D, GLFW_RELEASE), &CMovement::StopMoveRight, StopMoveRightEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_W, GLFW_PRESS), &CMovement::MoveForward, MoveForwardEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_W, GLFW_RELEASE), &CMovement::StopMoveForward, StopMoveForwardEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_S, GLFW_PRESS), &CMovement::MoveBack, MoveBackEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_S, GLFW_RELEASE), &CMovement::StopMoveBack, StopMoveBackEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS), &CMovement::MoveDown, MoveDownEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE), &CMovement::StopMoveDown, StopMoveDownEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_SPACE, GLFW_PRESS), &CMovement::MoveUp, MoveUpEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_SPACE, GLFW_RELEASE), &CMovement::StopMoveUp, StopMoveUpEvent);

	InputManager->RegisterMouseMoveEvent(this, &CMovement::Rotate, MouseRotateEvent);

	assert(ECS);
	TransformComponent = ECS->GetComponent<CTransformComponent>(EntityID);
	assert(TransformComponent);
}

void CMovement::MoveLeft()
{
	Movement.x -= 1.f;
}

void CMovement::MoveRight()
{
	Movement.x += 1.f;
}

void CMovement::StopMoveLeft()
{
	Movement.x += 1.f;
}

void CMovement::StopMoveRight()
{
	Movement.x -= 1.f;
}

void CMovement::MoveForward()
{
	Movement.z -= 1.f;
}

void CMovement::StopMoveForward()
{
	Movement.z += 1.f;
}

void CMovement::MoveBack()
{
	Movement.z += 1.f;
}

void CMovement::StopMoveBack()
{
	Movement.z -= 1.f;
}

void CMovement::MoveDown()
{
	Movement.y -= 1.f;
}

void CMovement::StopMoveDown()
{
	Movement.y += 1.f;
}

void CMovement::MoveUp()
{
	Movement.y += 1.f;
}

void CMovement::StopMoveUp()
{
	Movement.y -= 1.f;
}

void CMovement::Rotate(double X, double Y)
{
	TransformComponent->Rotate(glm::vec3(X, Y, 0) * 0.01f);
}

void CMovementSystem::RunComponent(CMovement& Component)
{
	assert(Component.GetTransform());

	if (glm::length(Component.Movement) > 0)
	{
		Component.GetTransform()->MoveRelative(glm::normalize(Component.Movement) * Component.Speed);
	}
}
