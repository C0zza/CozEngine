#include "Window.h"

#include <cassert>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void LWindow::Init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(800, 600, "CozEngine - GLFW Window", NULL, NULL);
	if (!m_Window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* Window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});

	glEnable(GL_DEPTH_TEST);
}

bool LWindow::ShouldClose() const
{
	assert(m_Window);
	return glfwWindowShouldClose(m_Window);
}
