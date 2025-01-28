#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "Globes.h"
#include "Misc/Logging.h"
#include "Subsystem.h"

struct GLFWwindow;
using KeyAction = std::pair<int, int>;

class LInputEventHandle
{
	friend class LInputManager;
public:
	LInputEventHandle() = default;
	~LInputEventHandle();

	// TODO: Add manual unbinding

private:
	static LInputManager* InputManager;
};

class LInputManager : public LSubsystem
{
private:
	enum class EInputEventType
	{
		Action,
		Mouse,
		None
	};

	struct LEventRefData
	{
		LEventRefData() = default;
		LEventRefData(const KeyAction& i_Ka, const int i_Index, const EInputEventType i_Type)
			: Ka{i_Ka}, Index{i_Index}, Type{i_Type}
		{
		}

		KeyAction Ka;
		int Index = -1;
		EInputEventType Type = EInputEventType::None;
	};

	std::map<const KeyAction, std::vector<std::function<void()>>> ActionEvents;
	std::vector<std::function<void(double, double)>> MouseMoveEvents;

	// To allow for RAII with LInputEventHandle
	std::map<LInputEventHandle*, const LEventRefData> EventRefs;

	bool IsInitialized = false;

	double PreviousMouseX = 0.f;
	double PreviousMouseY = 0.f;

	static std::map<void*, LInputManager*> InputManagers;

	GLFWwindow* GetWindow() const;

protected:
	virtual void Initialize() override;

public:
	static const std::map<void*, LInputManager*>& GetInputManagers() { return InputManagers; }

	void ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos);

	void OnCursorFocusChanged(GLFWwindow* window, int entered);
	bool bMouseFocused = false;

	void ResetMousePositionData();

#if defined(COZ_EDITOR)
	// TODO: This has a bug when using GLFW_CURSOR_DISABLED. For some reason the mouse is reset to an old position.
	// void OnInputEnabledChanged(const bool bEnabled);
	bool bInputEnabled = false;
#endif

	virtual bool HasRequiredSubsystems() const override;
	
	template<typename TCallerType, typename TFuncRef>
	void RegisterActionEvent(TCallerType* Caller, KeyAction iKeyAction, TFuncRef&& FuncRef, LInputEventHandle& EventHandle)
	{
		if (!Caller)
		{
			return;
		}

		const int KeyActionsVecSize = ActionEvents[iKeyAction].size();

		LEventRefData EventRefData(iKeyAction, KeyActionsVecSize, EInputEventType::Action);

		ActionEvents[iKeyAction].emplace_back(std::bind(FuncRef, Caller));
		EventRefs.emplace(&EventHandle, std::move(EventRefData));
	}

	template<typename TCallerType, typename TFuncRef>
	void RegisterMouseMoveEvent(TCallerType* Caller, TFuncRef&& FuncRef, LInputEventHandle& EventHandle)
	{
		if (!Caller)
		{
			return;
		}

		LEventRefData EventRefData(KeyAction(), MouseMoveEvents.size(), EInputEventType::Mouse);

		MouseMoveEvents.emplace_back(std::bind(FuncRef, Caller, std::placeholders::_1, std::placeholders::_2));
		EventRefs.emplace(&EventHandle, std::move(EventRefData));
	}

	void UnregisterHandle(LInputEventHandle* Handle);
};
