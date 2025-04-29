#pragma once

#include "ECS/Entity.h"
#include "Input/InputManager.h"

class CPlayerEntity : public LEntity
{
public:
	CPlayerEntity();

#if defined(COZ_EDITOR)
	virtual const char* GetTypeName() const override { return "CPlayerEntity"; }
#endif

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

#if defined(COZ_EDITOR)
	LInputEventHandle IncreaseSpeedEvent;
	LInputEventHandle DecreaseSpeedEvent;

	LInputEventHandle RightMousePressedEvent;
	LInputEventHandle RightMouseReleasedEvent;
	bool bRightMousePressed = false;

	void IncreaseSpeed();
	void DecreaseSpeed();

	void RightMousePressed();
	void RightMouseReleased();
#endif

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
REGISTER_ENTITY(CPlayerEntity)
