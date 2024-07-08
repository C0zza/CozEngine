#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

class LECS;

struct CCameraComponent : public LECSComponent
{
	glm::mat4 CachedViewMatrix;
};

class CCameraComponentSystem : public LComponentSystem<CCameraComponent>
{
public:
	const glm::mat4& GetViewMatrix();
	const glm::vec3& GetViewPos();

	void ActivateCamera(const CCameraComponent& Component) { ActiveCameraEntityID = Component.EntityID; }
	LEntityID GetActiveCameraEntityID() { return ActiveCameraEntityID; }

private:
	LEntityID ActiveCameraEntityID = -1;
};



