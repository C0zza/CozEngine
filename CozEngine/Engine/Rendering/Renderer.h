#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class Camera;
class Window;

class Renderer
{
public:
	void Init();
	void Shutdown();
	void Tick();

	std::shared_ptr<Window>& GetWindow() { return m_Window; }

	void SetActiveCamera(const std::shared_ptr<Camera>& i_Camera);

	// TEMP
	void PostTick();
	const glm::mat4& GetViewMatrix() const { return ViewMatrix; }
	const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }

private:
	std::shared_ptr<Window> m_Window;
	std::weak_ptr<Camera> m_ActiveCamera;

	glm::mat4 ViewMatrix = glm::mat4(0);
	glm::mat4 ProjectionMatrix = glm::mat4(0);
};

