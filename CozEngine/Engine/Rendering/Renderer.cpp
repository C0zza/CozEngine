#include "Renderer.h"

#include "Camera.h"
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::shared_ptr<Camera> ActiveCamera = m_ActiveCamera.lock();
	if (!ActiveCamera.get())
	{
		return;
	}

	ViewMatrix = ActiveCamera->GetViewMatrix();

	// Drawing stuff

	/*assert(m_Window->m_Window);
	glfwSwapBuffers(m_Window->m_Window);
	glfwPollEvents();*/
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
