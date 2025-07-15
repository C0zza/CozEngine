#pragma once

#include <glm/mat4x4.hpp>

#include "Subsystem.h"

class LCameraManager;
class LRenderer;

class LRendererInfo : public LSubsystem
{
public:
	virtual void Initialize() final;
	virtual void Deinitialize() final;

	const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }
	void UpdateProjectionMatrix(const int Width, const int Height);

	const glm::mat4& GetViewMatrix() const;
	const glm::vec3& GetViewPos();

	bool CanRender();

private:
	LCameraManager* GetCameraManager();

	glm::mat4 ProjectionMatrix = glm::mat4(0);

	LCameraManager* CameraManager = nullptr;
	LRenderer* Renderer = nullptr;
};

