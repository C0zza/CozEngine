#include "InputManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Window.h"

#include "Globes.h"
#include "imgui/imgui.h"

std::map<void*, LInputManager*> LInputManager::InputManagers{};

void LInputManager::Initialize()
{
	LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	assert(Renderer);
	LWindow* Window = Renderer->GetWindow();
	assert(Window);
	assert(Window->m_Window);

	if (LInputManager::GetInputManagers().contains(Window->m_Window))
	{
		Log(LLogLevel::WARNING, "LInputMangager::Initialize - Additional LInputManager created for window. Ignoring");
		return;
	}

	InputManagers.emplace(Window->m_Window, this);

	glfwSetKeyCallback(Window->m_Window, KeyCallback);

	glfwGetCursorPos(Window->m_Window, &PreviousMouseX, &PreviousMouseY);
	glfwSetCursorPosCallback(Window->m_Window, MouseMoveCallback);
	glfwSetInputMode(Window->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	IsInitialized = true;
}

void LInputManager::ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void LInputManager::ProcessMouseMove(GLFWwindow* window, double xpos, double ypos)
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

bool LInputManager::HasRequiredSubsystems() const
{
	return CSystem.GetSubsystems().GetSubsystem<LRenderer>();
}
