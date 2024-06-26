#pragma once

#include "Renderer.h"
#include "Input/InputManager.h"

struct GLFWwindow;

class LWindow
{
public:
	friend class LImGuiSubsystem;
	friend class LInputManager;
	friend class LRenderer;
	friend class LSystem;

	void Init();
	// void Shutdown(); Does m_Window need deleting or does GLFW handle it?

	bool ShouldClose() const;

	GLFWwindow* GetWindow() { return m_Window; }

private:
	GLFWwindow* m_Window;

	int Width = 1280;
	int Height = 720;
};

