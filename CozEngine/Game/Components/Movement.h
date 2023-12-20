#pragma once

#include <glm/vec3.hpp>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Input/InputManager.h"

struct CTransformComponent;

struct CMovement : LECSComponent
{
public:
	CMovement();

	virtual void Init() override;

	float Speed = 0.05f;

	glm::vec3 Movement = glm::vec3(0.f, 0.f, 0.f);

	CTransformComponent* GetTransform() { return TransformComponent; }

private:
	LKeyInputEvent<CMovement> MoveLeftEvent;
	LKeyInputEvent<CMovement> MoveRightEvent;
	LKeyInputEvent<CMovement> StopMoveLeftEvent;
	LKeyInputEvent<CMovement> StopMoveRightEvent;
	LKeyInputEvent<CMovement> MoveForwardEvent;
	LKeyInputEvent<CMovement> StopMoveForwardEvent;
	LKeyInputEvent<CMovement> MoveBackEvent;
	LKeyInputEvent<CMovement> StopMoveBackEvent;
	LKeyInputEvent<CMovement> MoveDownEvent;
	LKeyInputEvent<CMovement> StopMoveDownEvent;
	LKeyInputEvent<CMovement> MoveUpEvent;
	LKeyInputEvent<CMovement> StopMoveUpEvent;

	void MoveLeft();
	void StopMoveLeft();
	void MoveRight();
	void StopMoveRight();
	void MoveForward();
	void StopMoveForward();
	void MoveBack();
	void StopMoveBack();
	void MoveDown();
	void StopMoveDown();
	void MoveUp();
	void StopMoveUp();

	LMouseMoveEvent<CMovement> MouseRotateEvent;
	void Rotate(double X, double Y);

	// TODO: Pointer will become invalid if the component pool vector is moved :/
	CTransformComponent* TransformComponent;
};

class CMovementSystem : public LComponentSystem<CMovement>
{
public:
	CMovementSystem() { IsTickable = true; }

private:
	virtual void RunComponent(CMovement& Component) override;
};

