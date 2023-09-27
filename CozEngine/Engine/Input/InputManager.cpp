#include "InputManager.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Window.h"

LInputManager::InputEventList LInputManager::Events = {};
bool LInputManager::IsInitialized = false;

void LInputManager::Init(LWindow* i_Window)
{
	assert(i_Window);
	glfwSetKeyCallback(i_Window->m_Window, KeyCallback);
	IsInitialized = true;
}

void LInputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyAction Ka;
	Ka.first = key;
	Ka.second = action;

	if (Events.find(Ka) != Events.end())
	{
		for (unsigned int i = 0; i < Events[Ka].size(); i++)
		{
			(*Events[Ka][i])();
		}

		// TODO: Test for recursion? If event consumes input?
	}
  }
