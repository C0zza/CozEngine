#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

class Camera;
class CModelComponent;
class LWindow;

class Renderer
{
public:
	void Init();
	void Shutdown();
	void Tick();

	std::shared_ptr<LWindow>& GetWindow() { return m_Window; }

	// TEMP
	void PostTick();
	const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }

private:
	std::shared_ptr<LWindow> m_Window;

	glm::mat4 ProjectionMatrix = glm::mat4(0);
};

