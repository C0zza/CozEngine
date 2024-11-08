#include "InputManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Window.h"

#include "Development/LImGuiSubsystem.h"
#include "Globes.h"
#include "imgui/imgui.h"

std::map<void*, LInputManager*> LInputManager::InputManagers{};

namespace CE::InputManager
{
	void CursorEnterCallback(GLFWwindow* window, int entered)
	{
		assert(window);

		if (LInputManager::GetInputManagers().contains(window))
		{
			LInputManager::GetInputManagers().at(window)->OnCursorFocusChanged(window, entered);
		}
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (window && LInputManager::GetInputManagers().contains(window))
		{
			LInputManager::GetInputManagers().at(window)->ProcessKey(window, key, scancode, action, mods);
		}
	}

	void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (window && LInputManager::GetInputManagers().contains(window))
		{
			LInputManager::GetInputManagers().at(window)->ProcessMouseMove(window, xpos, ypos);
		}
	}
}

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

	glfwSetKeyCallback(Window->m_Window, CE::InputManager::KeyCallback);

	glfwGetCursorPos(Window->m_Window, &PreviousMouseX, &PreviousMouseY);
	glfwSetCursorPosCallback(Window->m_Window, CE::InputManager::MouseMoveCallback);
	glfwSetCursorEnterCallback(Window->m_Window, CE::InputManager::CursorEnterCallback);
	glfwSetInputMode(Window->m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_CURSOR_NORMAL);

	IsInitialized = true;
}

void LInputManager::ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
#if defined(COZ_EDITOR)
	if (CSystem.GetSubsystems().GetSubsystem<LImGuiSubsystem>() && ImGui::GetIO().WantCaptureKeyboard && !bEditorSceneFocused)
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
	if (!bMouseFocused)
	{
		return;
	}

#if defined(COZ_EDITOR)
	if (CSystem.GetSubsystems().GetSubsystem<LImGuiSubsystem>() && ImGui::GetIO().WantCaptureMouse && !bEditorSceneFocused)
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

void LInputManager::OnCursorFocusChanged(GLFWwindow* window, int entered)
{
	if (entered)
	{
		glfwGetCursorPos(window, &PreviousMouseX, &PreviousMouseY);
		bMouseFocused = true;
	}
	else
	{
		bMouseFocused = false;
	}
}

bool LInputManager::HasRequiredSubsystems() const
{
	return CSystem.GetSubsystems().GetSubsystem<LRenderer>();
}

void LInputManager::RegisterKeyEvent(const KeyAction& i_KeyAction, KeyEvent* i_Event)
{
	assert(i_Event);

	if (Events.find(i_KeyAction) == Events.end())
	{
		Events.insert({ i_KeyAction, {} });
	}

	std::vector<KeyEvent*>& KeyActionEvents = Events[i_KeyAction];

	std::vector<KeyEvent*>::iterator it = std::find(KeyActionEvents.begin(), KeyActionEvents.end(), i_Event);
	if (it == KeyActionEvents.end())
	{
		KeyActionEvents.push_back(i_Event);
	}
	else
	{
		// TODO: Check this makes sense. I.e. would i_Event point to the same place for multiple objects of the same type (assuming the same method).
		assert(false);
		Log(LLogLevel::INFO, "LInputManager::RegisterKeyEvent - Attempted to register duplicate KeyEvent. Ignoring.");
	}
}

void LInputManager::UnregisterKeyEvent(KeyEvent* i_Event)
{
	assert(i_Event);

	for (auto& Event : Events)
	{
		std::vector<KeyEvent*>::iterator it = std::find(Event.second.begin(), Event.second.end(), i_Event);
		if (it != Event.second.end())
		{
			Event.second.erase(it);
			return;
		}
	}

	Log(LLogLevel::WARNING, "LInputManager::UnregisterKeyEvent - Could not find KeyEvent to unregister.");
}

void LInputManager::RegisterMouseMoveEvent(MouseMoveEvent* i_Event)
{
	assert(i_Event);

	std::vector<MouseMoveEvent*>::iterator it = std::find(MouseMoveEvents.begin(), MouseMoveEvents.end(), i_Event);

	if (it == MouseMoveEvents.end())
	{
		MouseMoveEvents.push_back(i_Event);
	}
	else
	{
		Log(LLogLevel::INFO, "LInputManager::RegisterMouseMoveEvent - Attempted to register duplicate MouseMoveEvent. Ignoring.");
	}
}

void LInputManager::UnregisterMouseMoveEvent(MouseMoveEvent* i_Event)
{
	assert(i_Event);

	std::vector<MouseMoveEvent*>::iterator it = std::find(MouseMoveEvents.begin(), MouseMoveEvents.end(), i_Event);

	if (it != MouseMoveEvents.end())
	{
		MouseMoveEvents.erase(it);
	}
	else
	{
		Log(LLogLevel::WARNING, "LInputManager::UnregisterMouseMoveEvent - Could not find MouseMoveEvent to unregister.");
	}
}
