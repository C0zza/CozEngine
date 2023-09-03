#include "Renderer.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "Camera.h"
#include "Components/ModelComponent.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

void Renderer::Init()
{
	// Utility library for OpenGL
	glfwInit();

	m_Window = std::make_shared<Window>();
	assert(m_Window);
	m_Window->Init();

	ProjectionMatrix = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);
}

void Renderer::Shutdown()
{
	glfwTerminate();
}

void Renderer::Tick()
{
	glClearColor(.0f, .0f, .0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::shared_ptr<Camera> ActiveCamera = m_ActiveCamera.lock();
	if (!ActiveCamera.get())
	{
		return;
	}

	ViewMatrix = ActiveCamera->GetViewMatrix();
}

void Renderer::PostTick()
{
	assert(m_Window->m_Window);
	glfwSwapBuffers(m_Window->m_Window);
	glfwPollEvents();
}

void Renderer::SetActiveCamera(const std::shared_ptr<Camera>& i_Camera)
{
	if (i_Camera)
	{
		m_ActiveCamera = std::weak_ptr<Camera>(i_Camera);
	}
}
