#pragma once

#include <memory>

#include "Renderer.h"
#include "Input/InputManager.h"

struct GLFWwindow;

class Window
{
public:
	friend class InputManager;
	friend class Renderer;

	void Init();
	// void Shutdown(); Does m_Window need deleting or does GLFW handle it?

	bool ShouldClose() const;

private:
	GLFWwindow* m_Window;
};

