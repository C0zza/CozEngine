#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

#include "ECS/ECSComponents/ECSComponent.h"

class LECS;

struct CCameraComponent : public LECSComponent
{
	const glm::mat4& GetViewMatrix();
	const glm::vec3& GetViewPos();

	void ActivateCamera() { ActiveCameraEntityID = EntityID; }

	static LEntityID GetActiveCameraEntityID() { return ActiveCameraEntityID; }

private:
	glm::mat4 CachedViewMatrix;

	static LEntityID ActiveCameraEntityID;
};



