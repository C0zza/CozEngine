#include "Movement.h"

#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/TransformComponent.h"

CMovement::CMovement()
{
	// TODO: DANGER DANGER, high chance of initialising something here and then since the ECS currently copies components into the vector 
	// this will cause bad stuff. Should have initialisation in init methods
}

void CMovement::Init()
{
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


	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_A, GLFW_PRESS), &MoveLeftEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_A, GLFW_RELEASE), &StopMoveLeftEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_D, GLFW_PRESS), &MoveRightEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_D, GLFW_RELEASE), &StopMoveRightEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_W, GLFW_PRESS), &MoveForwardEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_W, GLFW_RELEASE), &StopMoveForwardEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_S, GLFW_PRESS), &MoveBackEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_S, GLFW_RELEASE), &StopMoveBackEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS), &MoveDownEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE), &StopMoveDownEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_SPACE, GLFW_PRESS), &MoveUpEvent);
	LInputManager::RegisterKeyEvent(KeyAction(GLFW_KEY_SPACE, GLFW_RELEASE), &StopMoveUpEvent);

	MouseRotateEvent.Init(this, &CMovement::Rotate);
	LInputManager::RegisterMouseMoveEvent(&MouseRotateEvent);

	TransformComponent = LECS::Get()->GetComponent<CTransformComponent>(EntityID);
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
