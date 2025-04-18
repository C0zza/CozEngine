#include "Window.h"

#include <cassert>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Misc/Logging.h"

void LWindow::Init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(Width, Height, "CozEngine - GLFW Window", NULL, NULL);
	if (!m_Window)
	{
		Log(LLogLevel::ERROR, "LWindow::Init - Failed to create GLFW window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log(LLogLevel::ERROR, "LWindow::Init - Failed to initialize GLAD");
		return;
	}

	glViewport(0, 0, Width, Height);
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* Window, int width, int height)
		{
#if !defined(COZ_EDITOR)
			LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
			if (Renderer)
			{
				Renderer->SetProjectionMatrix(width, height);
			}
#endif

			glViewport(0, 0, width, height);
		});

	glEnable(GL_DEPTH_TEST);
}

bool LWindow::ShouldClose() const
{
	assert(m_Window);
	return glfwWindowShouldClose(m_Window);
}
