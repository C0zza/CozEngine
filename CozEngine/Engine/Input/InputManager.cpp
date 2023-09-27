#include "InputManager.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Window.h"

void InputManager::Init(std::shared_ptr<LWindow> i_Window)
{
	m_Window = std::weak_ptr<LWindow>(i_Window);
}

void InputManager::ProcessInput()
{
	std::shared_ptr<LWindow> sWindow = m_Window.lock();
	assert(sWindow && sWindow->m_Window);

	if (glfwGetKey(sWindow->m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(sWindow->m_Window, true);
	}
}
