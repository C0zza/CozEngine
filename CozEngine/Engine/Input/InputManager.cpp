#include "InputManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

#include "Development/LImGuiSubsystem.h"
#include "Globes.h"
#include "imgui/imgui.h"

std::map<void*, LInputManager*> LInputManager::InputManagers{};

LInputManager* LInputEventHandle::InputManager = nullptr;

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
			LInputManager::GetInputManagers().at(window)->ProcessKey(window, key, action, mods);
		}
	}

	void MouseButtonCallback(GLFWwindow* window, int key, int action, int mods)
	{
		if (window && LInputManager::GetInputManagers().contains(window))
		{
			LInputManager::GetInputManagers().at(window)->ProcessKey(window, key, action, mods);
		}
	}

	void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (window && LInputManager::GetInputManagers().contains(window))
		{
			LInputManager* InputManager = LInputManager::GetInputManagers().at(window);

			if (yoffset > 0)
			{
				InputManager->ProcessKey(window, GLFW_MOUSE_SCROLL_UP, GLFW_PRESS, 0);
			}
			else if (yoffset < 0)
			{
				InputManager->ProcessKey(window, GLFW_MOUSE_SCROLL_DOWN, GLFW_PRESS, 0);
			}
		}
	}

	void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (window && LInputManager::GetInputManagers().contains(window))
		{
			LInputManager::GetInputManagers().at(window)->ProcessMouseMove(window, xpos, ypos);
		}
	}

	void WindowFocusCallback(GLFWwindow* window, int focused)
	{
		if (!window)
		{
			Log(LLogLevel::ERROR, "CE::InputManager::WindowFocusFallback - Invalid window");
			return;
		}

		if (!focused)
		{
			ImGui::SetWindowFocus(NULL);
		}
	}
}

GLFWwindow* LInputManager::GetWindow() const
{
	LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	LWindow* Window = Renderer ? Renderer->GetWindow() : nullptr;
	return Window ? Window->m_Window : nullptr;
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
	glfwSetMouseButtonCallback(Window->m_Window, CE::InputManager::MouseButtonCallback);
	glfwSetScrollCallback(Window->m_Window, CE::InputManager::MouseScrollCallback);

	glfwGetCursorPos(Window->m_Window, &PreviousMouseX, &PreviousMouseY);
	glfwSetCursorPosCallback(Window->m_Window, CE::InputManager::MouseMoveCallback);
	glfwSetCursorEnterCallback(Window->m_Window, CE::InputManager::CursorEnterCallback);
#if defined(COZ_EDITOR)
	glfwSetWindowFocusCallback(Window->m_Window, CE::InputManager::WindowFocusCallback);
#endif
	glfwSetInputMode(Window->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	LInputEventHandle::InputManager = this;

	IsInitialized = true;
}

LInputManager::LInputManager()
{
	MouseMoveEvents.reserve(MaxMouseEvents);
}

void LInputManager::ProcessKey(GLFWwindow* window, int key, int action, int mods)
{
#if defined(COZ_EDITOR)
	if (CSystem.GetSubsystems().GetSubsystem<LImGuiSubsystem>() && ImGui::GetIO().WantCaptureKeyboard && !bInputEnabled)
	{
		return;
	}
#endif

	KeyAction Ka;
	Ka.first = key;
	Ka.second = action;

	if (ActionEvents.contains(Ka))
	{
		for (std::function<void()>& Func : ActionEvents[Ka])
		{
			Func();
		}
	}
}

void LInputManager::ProcessMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	// TODO: Not sure if this is needed anymore. Behaviour doesn't seem to change when removed.
	if (!bMouseFocused)
	{
		return;
	}

#if defined(COZ_EDITOR)
	if (CSystem.GetSubsystems().GetSubsystem<LImGuiSubsystem>() && ImGui::GetIO().WantCaptureMouse && !bInputEnabled)
	{
		return;
	}
#endif

	double MovementX = xpos - PreviousMouseX;
	double MovementY = ypos - PreviousMouseY;

	for (std::function<void(double, double)>& Event : MouseMoveEvents)
	{
		Event(-MovementY, -MovementX);
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

void LInputManager::ResetMousePositionData()
{
	GLFWwindow* GlfwWindow = GetWindow();
	if (!GlfwWindow)
	{
		Log(LLogLevel::ERROR, "LInputManager::ResetMousePositionData - Unable to obtain GLFWwindow. Cannot reset mouse position data.");
		return;
	}

	glfwGetCursorPos(GlfwWindow, &PreviousMouseX, &PreviousMouseY);
}

//#if defined(COZ_EDITOR)
//void LInputManager::OnInputEnabledChanged(const bool bEnabled)
//{
//	GLFWwindow* GlfwWindow = GetWindow();
//	if (!GlfwWindow)
//	{
//		Log(LLogLevel::ERROR, "LInputManager::OnInputEnabledChanged - Unable to obtain GLFWwindow.");
//		return;
//	}
//
//	if (bEnabled)
//	{
//		glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	}
//	else
//	{
//		glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//	}
//}
//#endif

bool LInputManager::HasRequiredSubsystems() const
{
	return CSystem.GetSubsystems().GetSubsystem<LRenderer>();
}

void LInputManager::UnregisterHandle(LInputEventHandle* Handle)
{
	if (!EventRefs.contains(Handle))
	{
		return;
	}

	const LEventRefData& EventRefData = EventRefs[Handle];
	if (EventRefData.Index < 0)
	{
		return;
	}

	if (EventRefData.Type == EInputEventType::Action)
	{
		if (!ActionEvents.contains(EventRefData.Ka))
		{
			return;
		}

		std::vector<std::function<void()>>& Funcs = ActionEvents[EventRefData.Ka];

		RemoveIndex(Funcs, EventRefData.Index);

		if (Funcs.empty())
		{
			ActionEvents.erase(EventRefData.Ka);
		}
	}
	else if (EventRefData.Type == EInputEventType::Mouse)
	{
		RemoveIndex(MouseMoveEvents, EventRefData.Index);
	}

	EventRefs.erase(Handle);
}

LInputEventHandle::~LInputEventHandle()
{
	InputManager->UnregisterHandle(this);
}

void LInputEventHandle::Reset()
{
	InputManager->UnregisterHandle(this);
}
