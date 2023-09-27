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

	LKeyInputEvent<CTestComponent> MoveLeftEvent;
	LKeyInputEvent<CTestComponent> MoveRightEvent;
	LKeyInputEvent<CTestComponent> StopMoveLeftEvent;
	LKeyInputEvent<CTestComponent> StopMoveRightEvent;
	LKeyInputEvent<CTestComponent> MoveForwardEvent;
	LKeyInputEvent<CTestComponent> StopMoveForwardEvent;
	LKeyInputEvent<CTestComponent> MoveBackEvent;
	LKeyInputEvent<CTestComponent> StopMoveBackEvent;

	void MoveLeft();
	void StopMoveLeft();
	void MoveRight();
	void StopMoveRight();
	void MoveForward();
	void StopMoveForward();
	void MoveBack();
	void StopMoveBack();

	LMouseMoveEvent<CTestComponent> MouseRotateEvent;
	void Rotate(double X, double Y);

	glm::vec3 Movement = glm::vec3(0.f, 0.f, 0.f);
};

