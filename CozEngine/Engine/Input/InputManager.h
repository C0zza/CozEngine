#pragma once

#include <memory>

class LWindow;

class InputManager
{
public:
	void Init(std::shared_ptr<LWindow> i_Window);

	void ProcessInput();

private:
	std::weak_ptr<LWindow> m_Window;
};

