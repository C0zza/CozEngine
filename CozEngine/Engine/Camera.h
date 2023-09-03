#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

class LTransform;

class Camera
{
public:
	Camera();

	glm::mat4 GetViewMatrix() const;

	std::unique_ptr<LTransform> CameraTransform;
};