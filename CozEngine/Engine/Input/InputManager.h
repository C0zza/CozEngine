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

class LInputManager : public LSubsystem<LInputManager>
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

	virtual bool HasRequiredSubsystems() const override;

	void RegisterKeyEvent(const KeyAction& i_KeyAction, KeyEvent* i_Event)
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

	void UnregisterKeyEvent(KeyEvent* i_Event)
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

	void RegisterMouseMoveEvent(MouseMoveEvent* i_Event)
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

	void UnregisterMouseMoveEvent(MouseMoveEvent* i_Event)
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
};

inline void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (window && LInputManager::GetInputManagers().contains(window))
	{
		LInputManager::GetInputManagers().at(window)->ProcessKey(window, key, scancode, action, mods);
	}
}

inline void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (window && LInputManager::GetInputManagers().contains(window))
	{
		LInputManager::GetInputManagers().at(window)->ProcessMouseMove(window, xpos, ypos);
	}
}
