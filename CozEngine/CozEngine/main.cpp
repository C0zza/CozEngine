#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Rendering/Shader.h"

// Temp input function. Should eventually setup input system.
void ProcessInput(GLFWwindow* Window)
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
}

int main()
{
	glfwInit();

	// Window config info: http://www.glfw.org/docs/latest/window.html#window_hints

	// >= Open GL 3.3 required
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* Window = glfwCreateWindow(800, 600, "CozEngine - GLFW Window", NULL, NULL);
	if (!Window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(Window, [](GLFWwindow* Window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});

	float Vertices[2][9] =
	{
		{
			-0.9f, -0.5f, 0.0f,  // left 
			-0.0f, -0.5f, 0.0f,  // right
			-0.45f, 0.5f, 0.0f,  // top 
		},
		{
			0.0f, -0.5f, 0.0f,  // left
			0.9f, -0.5f, 0.0f,  // right
			0.45f, 0.5f, 0.0f   // top 
		}
	};

	unsigned int Indices[] = 
	{  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};  

	unsigned int VAOs[2];
	unsigned int VBOs[2];
	for (int i = 0; i < 2; ++i)
	{
		glGenVertexArrays(1, &VAOs[i]);
		glBindVertexArray(VAOs[i]);
		glGenBuffers(1, &VBOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[i]), Vertices[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	std::unique_ptr<Shader> DefaultShader(new Shader("Engine/Rendering/DefaultShaders/shader.vs", "Engine/Rendering/DefaultShaders/shader.fs"));
	DefaultShader->Use();

	while (!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 2; ++i)
		{
			glBindVertexArray(VAOs[i]);
			for (int j = 0; j < 9; ++j)
			{
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
		}

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}