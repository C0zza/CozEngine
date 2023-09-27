#pragma once

#include <cassert>
#include <map>
#include <utility>
#include <vector>

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

class LInputManager
{
private:
	static std::map<const KeyAction, std::vector<KeyEvent*>> Events;
	static std::vector<MouseMoveEvent*> MouseMoveEvents;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

	static bool IsInitialized;

	static double PreviousMouseX;
	static double PreviousMouseY;

public:
	static void Init(LWindow* i_Window);

	static void RegisterKeyEvent(const KeyAction& i_KeyAction, KeyEvent* i_Event)
	{
		assert(i_Event);

		if (Events.find(i_KeyAction) == Events.end())
		{
			Events.insert({ i_KeyAction, {} });
		}

		Events[i_KeyAction].push_back(i_Event);
	}

	static void UnregisterKeyEvent(KeyEvent* i_Event)
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
	}

	static void RegisterMouseMoveEvent(MouseMoveEvent* i_Event)
	{
		assert(i_Event);

		std::vector<MouseMoveEvent*>::iterator it = std::find(MouseMoveEvents.begin(), MouseMoveEvents.end(), i_Event);

		if (it == MouseMoveEvents.end())
		{
			MouseMoveEvents.push_back(i_Event);
		}
		else
		{
			assert(false);
		}
	}

	static void UnregisterMouseMoveEvent(MouseMoveEvent* i_Event)
	{
		assert(i_Event);

		std::vector<MouseMoveEvent*>::iterator it = std::find(MouseMoveEvents.begin(), MouseMoveEvents.end(), i_Event);
		
		if (it != MouseMoveEvents.end())
		{
			MouseMoveEvents.erase(it);
		}
	}
};
