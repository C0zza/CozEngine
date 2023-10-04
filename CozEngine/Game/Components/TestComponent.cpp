#include "TestComponent.h"

#include <GLFW/glfw3.h>

#include "Object.h"
#include "Transform.h"

CTestComponent::CTestComponent()
{
	MoveLeftEvent.Init(this, &CTestComponent::MoveLeft);
	StopMoveLeftEvent.Init(this, &CTestComponent::StopMoveLeft);
	MoveRightEvent.Init(this, &CTestComponent::MoveRight);
	StopMoveRightEvent.Init(this, &CTestComponent::StopMoveRight);
	MoveForwardEvent.Init(this, &CTestComponent::MoveForward);
	StopMoveForwardEvent.Init(this, &CTestComponent::StopMoveForward);
	MoveBackEvent.Init(this, &CTestComponent::MoveBack);
	StopMoveBackEvent.Init(this, &CTestComponent::StopMoveBack);
	MoveDownEvent.Init(this, &CTestComponent::MoveDown);
	StopMoveDownEvent.Init(this, &CTestComponent::StopMoveDown);
	MoveUpEvent.Init(this, &CTestComponent::MoveUp);
	StopMoveUpEvent.Init(this, &CTestComponent::StopMoveUp);


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

	MouseRotateEvent.Init(this, &CTestComponent::Rotate);
	LInputManager::RegisterMouseMoveEvent(&MouseRotateEvent);
}

CTestComponent::~CTestComponent()
{
	LInputManager::UnregisterKeyEvent(&MoveLeftEvent);
	LInputManager::UnregisterKeyEvent(&StopMoveLeftEvent);
	LInputManager::UnregisterKeyEvent(&MoveRightEvent);
	LInputManager::UnregisterKeyEvent(&StopMoveRightEvent);
	LInputManager::UnregisterKeyEvent(&MoveForwardEvent);
	LInputManager::UnregisterKeyEvent(&StopMoveForwardEvent);
	LInputManager::UnregisterKeyEvent(&MoveBackEvent);
	LInputManager::UnregisterKeyEvent(&StopMoveBackEvent);
	LInputManager::UnregisterKeyEvent(&MoveDownEvent);
	LInputManager::UnregisterKeyEvent(&StopMoveDownEvent);
	LInputManager::UnregisterKeyEvent(&MoveUpEvent);
	LInputManager::UnregisterKeyEvent(&StopMoveUpEvent);

	LInputManager::UnregisterMouseMoveEvent(&MouseRotateEvent);
}

void CTestComponent::Tick()
{
	assert(Parent);
	
	if (glm::length(Movement) > 0)
	{
		Parent->Transform.MoveRelative(glm::normalize(Movement) * Speed);
	}
}

void CTestComponent::MoveLeft()
{
	Movement.x -= 1.f;
}

void CTestComponent::MoveRight()
{
	Movement.x += 1.f;
}

void CTestComponent::StopMoveLeft()
{
	Movement.x += 1.f;
}

void CTestComponent::StopMoveRight()
{
	Movement.x -= 1.f;
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

void CTestComponent::Rotate(double X, double Y)
{
	assert(Parent);
	Parent->Transform.Rotate(glm::vec3(X, Y, 0) * 0.01f);
}

void CTestComponent::MoveDown()
{
	Movement.y -= 1.f;
}

void CTestComponent::StopMoveDown()
{
	Movement.y += 1.f;
}

void CTestComponent::MoveUp()
{
	Movement.y += 1.f;
}

void CTestComponent::StopMoveUp()
{
	Movement.y -= 1.f;
}
