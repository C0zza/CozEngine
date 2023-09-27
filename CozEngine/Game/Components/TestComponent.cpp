#include "TestComponent.h"

#include <GLFW/glfw3.h>

#include "Object.h"
#include "Transform.h"

CTestComponent::CTestComponent()
{
	MoveLeftEvent = LInputEvent<CTestComponent>(this, &CTestComponent::MoveLeft);
	StopMoveLeftEvent = LInputEvent<CTestComponent>(this, &CTestComponent::StopMoveLeft);
	MoveRightEvent = LInputEvent<CTestComponent>(this, &CTestComponent::MoveRight);
	StopMoveRightEvent = LInputEvent<CTestComponent>(this, &CTestComponent::StopMoveRight);
	MoveForwardEvent = LInputEvent<CTestComponent>(this, &CTestComponent::MoveForward);
	StopMoveForwardEvent = LInputEvent<CTestComponent>(this, &CTestComponent::StopMoveForward);
	MoveBackEvent = LInputEvent<CTestComponent>(this, &CTestComponent::MoveBack);
	StopMoveBackEvent = LInputEvent<CTestComponent>(this, &CTestComponent::StopMoveBack);

	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_A, GLFW_PRESS), &MoveLeftEvent);
	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_A, GLFW_RELEASE), &StopMoveLeftEvent);
	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_D, GLFW_PRESS), &MoveRightEvent);
	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_D, GLFW_RELEASE), &StopMoveRightEvent);
	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_W, GLFW_PRESS), &MoveForwardEvent);
	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_W, GLFW_RELEASE), &StopMoveForwardEvent);
	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_S, GLFW_PRESS), &MoveBackEvent);
	LInputManager::RegisterEvent(KeyAction(GLFW_KEY_S, GLFW_RELEASE), &StopMoveBackEvent);
}

CTestComponent::~CTestComponent()
{
	LInputManager::UnregisterEvent(&MoveLeftEvent);
	LInputManager::UnregisterEvent(&StopMoveLeftEvent);
	LInputManager::UnregisterEvent(&MoveRightEvent);
	LInputManager::UnregisterEvent(&StopMoveRightEvent);
	LInputManager::UnregisterEvent(&MoveForwardEvent);
	LInputManager::UnregisterEvent(&StopMoveForwardEvent);
	LInputManager::UnregisterEvent(&MoveBackEvent);
	LInputManager::UnregisterEvent(&StopMoveBackEvent);
}

void CTestComponent::Tick()
{
	assert(Parent);
	
	if (glm::length(Movement) > 0)
	{
		Parent->Transform.Move(glm::normalize(Movement) * Speed);
	}
}

void CTestComponent::MoveLeft()
{
	Movement.x -= 1.f;
	assert(Movement.x >= -1.f);
}

void CTestComponent::MoveRight()
{
	Movement.x += 1.f;
	assert(Movement.x <= 1.f);
}

void CTestComponent::StopMoveLeft()
{
	Movement.x += 1.f;
	assert(Movement.x <= 1.f);
}

void CTestComponent::StopMoveRight()
{
	Movement.x -= 1.f;
	assert(Movement.x >= -1.f);
}

void CTestComponent::MoveForward()
{
	Movement.z -= 1.f;
}

void CTestComponent::StopMoveForward()
{
	Movement.z += 1.f;
}

void CTestComponent::MoveBack()
{
	Movement.z += 1.f;
}

void CTestComponent::StopMoveBack()
{
	Movement.z -= 1.f;
}
