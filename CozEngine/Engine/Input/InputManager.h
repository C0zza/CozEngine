#pragma once

#include <cassert>
#include <map>
#include <utility>
#include <vector>

struct GLFWwindow;
class LWindow;

using KeyAction = std::pair<int, int>;

class IInputEvent
{
public:
	virtual void operator() () = 0;
};

template<typename T>
class LInputEvent : public IInputEvent
{
	using KeyActionEvent = void (T::*)();
public:
	LInputEvent() {}

	LInputEvent(T* i_Object, KeyActionEvent i_Event)
		: Object{i_Object}, Event{i_Event}
	{
		assert(Object);
	}

	virtual void operator () () override { (Object->*Event)(); }

private:
	KeyActionEvent Event = nullptr;
	T* Object = nullptr;
};

class LInputManager
{
public:
	static void Init(LWindow* i_Window);

	template<typename T>
	static void RegisterEvent(const KeyAction& i_KeyAction, LInputEvent<T>* i_Event)
	{
		assert(IsInitialized);
		assert(i_Event);

		if (Events.find(i_KeyAction) == Events.end())
		{
			Events.insert({ i_KeyAction, {} });
		}

		Events[i_KeyAction].push_back(i_Event);
	}

	template<typename T>
	static void UnregisterEvent(LInputEvent<T>* i_Event)
	{
		assert(IsInitialized);
		assert(i_Event);

		for (auto& Event : Events)
		{
			std::vector<IInputEvent*>::iterator it = std::find(Event.second.begin(), Event.second.end(), i_Event);
			if (it != Event.second.end())
			{
				Event.second.erase(it);
				return;
			}
		}
	}

private:
	typedef std::map<const KeyAction, std::vector<IInputEvent*>> InputEventList;
	static InputEventList Events;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool IsInitialized;
};
