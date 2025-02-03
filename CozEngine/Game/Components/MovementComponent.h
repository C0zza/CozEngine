#pragma once

#include <glm/vec3.hpp>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

struct CTransformComponent;

struct CMovementComponent : public LECSComponent
{
	glm::vec3 Movement = glm::vec3(0.f, 0.f, 0.f);
	float Speed = 0.05f;

	glm::vec3 Rotation = glm::vec3(0.f, 0.f, 0.f);
	float Sensitivity = 0.01f;
};

class CMovementSystem : public LComponentSystem<CMovementComponent>
{
public:
	virtual void Init() override { IsTickable = true; }

private:
	virtual void RunComponent(CMovementComponent& Component) override;
};

