#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "Misc/Logging.h"
#include "Subsystem.h"

struct GLFWwindow;
class LWindow;

template<typename... CallbackArgs>
class IInputEvent
{
public:
	virtual void operator() (CallbackArgs... Args) = 0;
};

template<typename ParentType, typename Callback, typename... CallbackArgs>
class LInputEvent : public IInputEvent<CallbackArgs...>
{
public:
	LInputEvent() {}

	void Init(ParentType* i_Object, Callback i_Event)
	{
		Object = i_Object;
		Event = i_Event;
	}

	virtual void operator () (CallbackArgs... Args) override { (Object->*Event)(Args...); }

protected:
	Callback Event = nullptr;
	ParentType* Object = nullptr;
};

using KeyAction = std::pair<int, int>;

template<typename T>
using LKeyInputEvent = LInputEvent<T, void (T::*)()>;

template<typename T>
using LMouseMoveEvent = LInputEvent<T, void (T::*)(double X, double Y), double, double>;

using KeyEvent = IInputEvent<>;
using MouseMoveEvent = IInputEvent<double, double>;

class LInputManager : public LSubsystem
{
private:
	std::map<const KeyAction, std::vector<KeyEvent*>> Events;
	std::vector<MouseMoveEvent*> MouseMoveEvents;

	bool IsInitialized = false;

	double PreviousMouseX = 0.f;
	double PreviousMouseY = 0.f;

	static std::map<void*, LInputManager*> InputManagers;

protected:
	virtual void Initialize() override;

public:
	static const std::map<void*, LInputManager*>& GetInputManagers() { return InputManagers; }

	void ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	void ProcessMouseMove(GLFWwindow* window, double xpos, double ypos);

	void OnCursorFocusChanged(GLFWwindow* window, int entered);
	bool bMouseFocused = false;

#if defined(COZ_EDITOR)
	bool bEditorSceneFocused = false;
#endif

	virtual bool HasRequiredSubsystems() const override;

	void RegisterKeyEvent(const KeyAction& i_KeyAction, KeyEvent* i_Event);
	void UnregisterKeyEvent(KeyEvent* i_Event);

	void RegisterMouseMoveEvent(MouseMoveEvent* i_Event);
	void UnregisterMouseMoveEvent(MouseMoveEvent* i_Event);
};
