#include "Camera.h"
#include "Transform.h"
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera()
{
	CameraTransform = std::make_unique<Transform>();
}

glm::mat4 Camera::GetViewMatrix() const
{
	glm::vec3 ForwardVector = glm::vec4(0.f, 0.f, -1.f, 0.f);

	const glm::vec3& CameraRotation = CameraTransform->GetRotation();
	glm::mat4 RotationMatrix = glm::mat4(1.f);
	RotationMatrix = glm::rotate(RotationMatrix, glm::radians(CameraRotation.x), glm::vec3(1.f, 0.f, 0.f));
	RotationMatrix = glm::rotate(RotationMatrix, glm::radians(CameraRotation.y), glm::vec3(0.f, 1.f, 0.f));
	RotationMatrix = glm::rotate(RotationMatrix, glm::radians(CameraRotation.z), glm::vec3(0.f, 0.f, 1.f));

	glm::vec4 Target = RotationMatrix * glm::vec4(ForwardVector, 1.0f);

	return glm::lookAt(CameraTransform->GetPosition(), glm::vec3(Target.x, Target.y, Target.z), glm::vec3(0.f, 1.f, 0.f));
}
