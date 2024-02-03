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
	LInputManager* InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>();
	if (!InputManager)
	{
		Log(LLogLevel::WARNING, "CMovement::Init - Failed to init component after invalid LinputManager.");
		return;
	}

	MoveLeftEvent.Init(this, &CMovement::MoveLeft);
	StopMoveLeftEvent.Init(this, &CMovement::StopMoveLeft);
	MoveRightEvent.Init(this, &CMovement::MoveRight);
	StopMoveRightEvent.Init(this, &CMovement::StopMoveRight);
	MoveForwardEvent.Init(this, &CMovement::MoveForward);
	StopMoveForwardEvent.Init(this, &CMovement::StopMoveForward);
	MoveBackEvent.Init(this, &CMovement::MoveBack);
	StopMoveBackEvent.Init(this, &CMovement::StopMoveBack);
	MoveDownEvent.Init(this, &CMovement::MoveDown);
	StopMoveDownEvent.Init(this, &CMovement::StopMoveDown);
	MoveUpEvent.Init(this, &CMovement::MoveUp);
	StopMoveUpEvent.Init(this, &CMovement::StopMoveUp);

	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_A, GLFW_PRESS), &MoveLeftEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_A, GLFW_RELEASE), &StopMoveLeftEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_D, GLFW_PRESS), &MoveRightEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_D, GLFW_RELEASE), &StopMoveRightEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_W, GLFW_PRESS), &MoveForwardEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_W, GLFW_RELEASE), &StopMoveForwardEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_S, GLFW_PRESS), &MoveBackEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_S, GLFW_RELEASE), &StopMoveBackEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS), &MoveDownEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE), &StopMoveDownEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_SPACE, GLFW_PRESS), &MoveUpEvent);
	InputManager->RegisterKeyEvent(KeyAction(GLFW_KEY_SPACE, GLFW_RELEASE), &StopMoveUpEvent);

	MouseRotateEvent.Init(this, &CMovement::Rotate);
	InputManager->RegisterMouseMoveEvent(&MouseRotateEvent);

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
