#pragma once

#include <memory>

class Window;

class InputManager
{
public:
	void Init(std::shared_ptr<Window> i_Window);

	void ProcessInput();

private:
	std::weak_ptr<Window> m_Window;
};

