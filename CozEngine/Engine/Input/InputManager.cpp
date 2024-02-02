#include "InputManager.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Window.h"

#include "imgui/imgui.h"

std::map<const KeyAction, std::vector<KeyEvent*>> LInputManager::Events{};
std::vector<MouseMoveEvent*> LInputManager::MouseMoveEvents{};

bool LInputManager::IsInitialized = false;

double LInputManager::PreviousMouseX = 0.f;
double LInputManager::PreviousMouseY = 0.f;

void LInputManager::Init(LWindow* i_Window)
{
	assert(i_Window);
	glfwSetKeyCallback(i_Window->m_Window, KeyCallback);

	glfwGetCursorPos(i_Window->m_Window, &PreviousMouseX, &PreviousMouseY);
	glfwSetCursorPosCallback(i_Window->m_Window, MouseMoveCallback);
	glfwSetInputMode(i_Window->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	IsInitialized = true;
}

void LInputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
#if defined(COZ_EDITOR)
	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
#endif

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

void LInputManager::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
#if defined(COZ_EDITOR)
	if (ImGui::GetIO().WantCaptureMouse)
	{
		return;
	}
#endif

	double MovementX = xpos - PreviousMouseX;
	double MovementY = ypos - PreviousMouseY;

	for (auto& Event : MouseMoveEvents)
	{
		(*Event)(-MovementY, -MovementX);
	}

	PreviousMouseX = xpos;
	PreviousMouseY = ypos;
}
