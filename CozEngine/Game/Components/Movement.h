#pragma once

#include <glm/vec3.hpp>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Input/InputManager.h"

struct CTransformComponent;

struct CMovement : public LECSComponent
{
public:
	CMovement();

	float Speed = 0.05f;

	glm::vec3 Movement = glm::vec3(0.f, 0.f, 0.f);

	CTransformComponent* GetTransform() { return TransformComponent; }

protected:
	virtual void Init() override;

private:
	LInputEventHandle MoveLeftEvent;
	LInputEventHandle MoveRightEvent;
	LInputEventHandle StopMoveLeftEvent;
	LInputEventHandle StopMoveRightEvent;
	LInputEventHandle MoveForwardEvent;
	LInputEventHandle StopMoveForwardEvent;
	LInputEventHandle MoveBackEvent;
	LInputEventHandle StopMoveBackEvent;
	LInputEventHandle MoveDownEvent;
	LInputEventHandle StopMoveDownEvent;
	LInputEventHandle MoveUpEvent;
	LInputEventHandle StopMoveUpEvent;

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

	LInputEventHandle MouseRotateEvent;
	void Rotate(double X, double Y);

	// TODO: Pointer will become invalid if the component pool vector is moved :/
	CTransformComponent* TransformComponent;
};

class CMovementSystem : public LComponentSystem<CMovement>
{
public:
	virtual void Init() override { IsTickable = true; }

private:
	virtual void RunComponent(CMovement& Component) override;
};

