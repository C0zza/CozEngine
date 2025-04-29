#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "Globes.h"
#include "Misc/Logging.h"
#include "Subsystem.h"

#define GLFW_MOUSE_SCROLL_UP GLFW_MOUSE_BUTTON_LAST + 1
#define GLFW_MOUSE_SCROLL_DOWN GLFW_MOUSE_BUTTON_LAST + 2

struct GLFWwindow;
using KeyAction = std::pair<int, int>;

class LInputEventHandle
{
	friend class LInputManager;
public:
	LInputEventHandle() = default;
	~LInputEventHandle();

	void Reset();

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

	static constexpr int MaxMouseEvents = 5;
	std::vector<std::function<void(double, double)>> MouseMoveEvents;

	std::map<LInputEventHandle*, LEventRefData> EventRefs;
	std::map<void*, LInputEventHandle*> FuncToHandleMap;

	bool IsInitialized = false;

	double PreviousMouseX = 0.f;
	double PreviousMouseY = 0.f;

	static std::map<void*, LInputManager*> InputManagers;

	GLFWwindow* GetWindow() const;

	template<typename TVectorType>
	void RemoveIndex(std::vector<TVectorType>& Vector, const int IndexToRemove)
	{
		if (Vector.size() > 0 && IndexToRemove < Vector.size())
		{
			const int LastIndex = Vector.size() - 1;
			if (IndexToRemove != LastIndex)
			{
				TVectorType& LastRef = Vector[LastIndex];
				TVectorType& RemovedRef = Vector[IndexToRemove];

				EventRefs[FuncToHandleMap[&LastRef]].Index = IndexToRemove;
				FuncToHandleMap[&RemovedRef] = FuncToHandleMap[&LastRef];
				FuncToHandleMap.erase(&Vector[LastIndex]);
				Vector[IndexToRemove] = Vector[LastIndex];
			}
			else
			{
				FuncToHandleMap.erase(&Vector[IndexToRemove]);
			}

			Vector.erase(Vector.begin() + LastIndex);
		}
	}

protected:
	virtual void Initialize() override;

public:
	LInputManager();

	static const std::map<void*, LInputManager*>& GetInputManagers() { return InputManagers; }

	void ProcessKey(GLFWwindow* window, int key, int action, int mods);
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

		if (EventRefs.contains(&EventHandle))
		{
			Log(LLogLevel::WARNING, "LInputManager::RegisterActionEvent - EventHandle already registered. Reset it first.");
			return;
		}

		if (!ActionEvents.contains(iKeyAction))
		{
			ActionEvents[iKeyAction].reserve(5);
		}
		else if (ActionEvents[iKeyAction].size() >= 50)
		{
			Log(LLogLevel::ERROR, "LInputManager::RegisterActionEvent - vector reallocation will invalidate existing bindings. Ignoring register request.");
			return;
		}

		const int KeyActionsVecSize = ActionEvents[iKeyAction].size();

		LEventRefData EventRefData(iKeyAction, KeyActionsVecSize, EInputEventType::Action);

		ActionEvents[iKeyAction].emplace_back(std::bind(FuncRef, Caller));
		EventRefs.emplace(&EventHandle, std::move(EventRefData));

		std::function<void()>& FuncAdd = ActionEvents[iKeyAction][ActionEvents[iKeyAction].size() - 1];
		FuncToHandleMap.emplace((void*)&FuncAdd, &EventHandle);
	}

	template<typename TCallerType, typename TFuncRef>
	void RegisterMouseMoveEvent(TCallerType* Caller, TFuncRef&& FuncRef, LInputEventHandle& EventHandle)
	{
		if (!Caller)
		{
			return;
		}

		if (EventRefs.contains(&EventHandle))
		{
			Log(LLogLevel::WARNING, "LInputManager::RegisterActionEvent - EventHandle already registered. Reset it first.");
			return;
		}

		if (MouseMoveEvents.size() >= MaxMouseEvents)
		{
			Log(LLogLevel::ERROR, "LInputManager::RegisterMouseEvent - vector reallocation will invalidate existing bindings. Ignoring register request.");
			return;
		}

		LEventRefData EventRefData(KeyAction(), MouseMoveEvents.size(), EInputEventType::Mouse);

		MouseMoveEvents.emplace_back(std::bind(FuncRef, Caller, std::placeholders::_1, std::placeholders::_2));
		EventRefs.emplace(&EventHandle, std::move(EventRefData));

		std::function<void(double,double)>& FuncAdd = MouseMoveEvents[MouseMoveEvents.size() - 1];
		FuncToHandleMap.emplace((void*)&FuncAdd, &EventHandle);
	}

	void UnregisterHandle(LInputEventHandle* Handle);
};
