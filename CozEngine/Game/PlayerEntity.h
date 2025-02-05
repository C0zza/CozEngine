#pragma once

#include "GameFramework/EntityRegistration.h"
#include "ECS/Entity.h"
#include "Input/InputManager.h"

class CPlayerEntity : public LEntity
{
public:
	CPlayerEntity();

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

	LInputEventHandle MouseRotateEvent;

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

	void Rotate(double X, double Y);

	struct CMovementComponent* MovementComponent = nullptr;
	struct CTransformComponent* TransformComponent = nullptr;
};
REGISTER_ENTITY(CPlayerEntity);
