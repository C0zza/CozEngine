#pragma once

#include <memory>

#include "Renderer.h"
#include "Input/InputManager.h"

struct GLFWwindow;

class LWindow
{
public:
	friend class LInputManager;
	friend class Renderer;
	friend class System;

	void Init();
	// void Shutdown(); Does m_Window need deleting or does GLFW handle it?

	bool ShouldClose() const;

private:
	GLFWwindow* m_Window;
};

