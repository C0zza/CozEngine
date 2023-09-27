#pragma once

#include <glm/vec3.hpp>

#include "Engine/Components/Component.h"
#include "Input/InputManager.h"

class CTestComponent : public CComponent
{
public:
	CTestComponent();
	virtual ~CTestComponent();

	virtual void Tick() override;

private:
	float Speed = 0.05f;

	LInputEvent<CTestComponent> MoveLeftEvent;
	LInputEvent<CTestComponent> MoveRightEvent;
	LInputEvent<CTestComponent> StopMoveLeftEvent;
	LInputEvent<CTestComponent> StopMoveRightEvent;
	LInputEvent<CTestComponent> MoveForwardEvent;
	LInputEvent<CTestComponent> StopMoveForwardEvent;
	LInputEvent<CTestComponent> MoveBackEvent;
	LInputEvent<CTestComponent> StopMoveBackEvent;

	void MoveLeft();
	void StopMoveLeft();
	void MoveRight();
	void StopMoveRight();
	void MoveForward();
	void StopMoveForward();
	void MoveBack();
	void StopMoveBack();

	glm::vec3 Movement = glm::vec3(0.f, 0.f, 0.f);
};

