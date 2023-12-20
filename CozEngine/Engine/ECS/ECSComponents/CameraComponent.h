#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

class LECS;

struct CCameraComponent : LECSComponent
{
	const glm::mat4& GetViewMatrix();
	const glm::vec3& GetViewPos();

	void ActivateCamera() { ActiveCameraEntityID = EntityID; }

	static LEntityID GetActiveCameraEntityID() { return ActiveCameraEntityID; }

private:
	glm::mat4 CachedViewMatrix = glm::mat4(0.f);

	static LEntityID ActiveCameraEntityID;
};



