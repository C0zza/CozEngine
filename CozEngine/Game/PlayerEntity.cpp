#include "PlayerEntity.h"

#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Game/Components/MovementComponent.h"

CPlayerEntity::CPlayerEntity()
{
	CCameraComponent* CameraComponent = AddComponent<CCameraComponent>();
	if (CameraComponent)
	{
		CCameraComponentSystem* CameraComponentSys = nullptr;
		if (LECS* ECS = GetECS())
		{
			CameraComponentSys = dynamic_cast<CCameraComponentSystem*>(ECS->GetComponentSystemFor<CCameraComponent>());
		}

		if (CameraComponentSys)
		{
			CameraComponentSys->ActivateCamera(*CameraComponent);
		}
		else
		{
			Log(LLogLevel::WARNING, "CPlayerEntity::CPlayerEntity - Failed to activate camera");
		}
	}
	else
	{
		Log(LLogLevel::WARNING, "CPlayerEntity::CPlayerEntity - Failed to add camera component");
	}

	TransformComponent = GetComponent<CTransformComponent>();
	TransformComponent->Move(glm::vec3(0.f, 0.f, 10.0f));

	MovementComponent = AddComponent<CMovementComponent>();
	MovementComponent->Speed = 0.5f;

	LInputManager* InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>(true);
	if (!InputManager)
	{
		Log(LLogLevel::WARNING, "CPlayerEntity::CPlayerEntity - Failed to init component after invalid LinputManager.");
		return;
	}

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_A, GLFW_PRESS), &CPlayerEntity::MoveLeft, MoveLeftEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_A, GLFW_RELEASE), &CPlayerEntity::StopMoveLeft, StopMoveLeftEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_D, GLFW_PRESS), &CPlayerEntity::MoveRight, MoveRightEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_D, GLFW_RELEASE), &CPlayerEntity::StopMoveRight, StopMoveRightEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_W, GLFW_PRESS), &CPlayerEntity::MoveForward, MoveForwardEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_W, GLFW_RELEASE), &CPlayerEntity::StopMoveForward, StopMoveForwardEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_S, GLFW_PRESS), &CPlayerEntity::MoveBack, MoveBackEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_S, GLFW_RELEASE), &CPlayerEntity::StopMoveBack, StopMoveBackEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS), &CPlayerEntity::MoveDown, MoveDownEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE), &CPlayerEntity::StopMoveDown, StopMoveDownEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_SPACE, GLFW_PRESS), &CPlayerEntity::MoveUp, MoveUpEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_SPACE, GLFW_RELEASE), &CPlayerEntity::StopMoveUp, StopMoveUpEvent);
	
#if defined(COZ_EDITOR)
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS), &CPlayerEntity::RightMousePressed, RightMousePressedEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE), &CPlayerEntity::RightMouseReleased, RightMouseReleasedEvent);

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_SCROLL_UP, GLFW_PRESS), &CPlayerEntity::IncreaseSpeed, IncreaseSpeedEvent);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_SCROLL_DOWN, GLFW_PRESS), &CPlayerEntity::DecreaseSpeed, DecreaseSpeedEvent);
#else
	InputManager->RegisterMouseMoveEvent(this, &CPlayerEntity::Rotate, MouseRotateEvent);
#endif
}

void CPlayerEntity::MoveLeft()
{
	MovementComponent->Movement.x -= 1.f;
}

void CPlayerEntity::MoveRight()
{
	MovementComponent->Movement.x += 1.f;
}

void CPlayerEntity::StopMoveLeft()
{
	MovementComponent->Movement.x += 1.f;
}

void CPlayerEntity::StopMoveRight()
{
	MovementComponent->Movement.x -= 1.f;
}

void CPlayerEntity::MoveForward()
{
	MovementComponent->Movement.z -= 1.f;
}

void CPlayerEntity::StopMoveForward()
{
	MovementComponent->Movement.z += 1.f;
}

void CPlayerEntity::MoveBack()
{
	MovementComponent->Movement.z += 1.f;
}

void CPlayerEntity::StopMoveBack()
{
	MovementComponent->Movement.z -= 1.f;
}

void CPlayerEntity::MoveDown()
{
	MovementComponent->Movement.y -= 1.f;
}

void CPlayerEntity::StopMoveDown()
{
	MovementComponent->Movement.y += 1.f;
}

void CPlayerEntity::MoveUp()
{
	MovementComponent->Movement.y += 1.f;
}

void CPlayerEntity::StopMoveUp()
{
	MovementComponent->Movement.y -= 1.f;
}

#if defined(COZ_EDITOR)
void CPlayerEntity::IncreaseSpeed()
{
	MovementComponent->Speed += 0.1f;
}

void CPlayerEntity::DecreaseSpeed()
{
	MovementComponent->Speed -= 0.1f;

	if (MovementComponent->Speed < 0)
	{
		MovementComponent->Speed = 0.0f;
	}
}

void CPlayerEntity::RightMousePressed()
{
	LInputManager* InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>(true);
	if (!InputManager)
	{
		Log(LLogLevel::WARNING, "CPlayerEntity::CPlayerEntity - Failed to init component after invalid LinputManager.");
		return;
	}

	InputManager->RegisterMouseMoveEvent(this, &CPlayerEntity::Rotate, MouseRotateEvent);
}

void CPlayerEntity::RightMouseReleased()
{
	MouseRotateEvent.Reset();
}
#endif

void CPlayerEntity::Rotate(double X, double Y)
{
	TransformComponent->Rotate(glm::vec3(X, Y, 0) * 0.01f);
}