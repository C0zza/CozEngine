#include "System.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Transform.h"

#include <iostream>

System::System()
	: Window{ nullptr }
{
}

System::~System()
{
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	// glDeleteBuffers(1, &EBO);

	glfwTerminate();
}

void System::Init()
{
	glfwInit();

	// Window config info: http://www.glfw.org/docs/latest/window.html#window_hints

	// >= Open GL 3.3 required
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window = glfwCreateWindow(800, 600, "CozEngine - GLFW Window", NULL, NULL);
	if (!Window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(Window, [](GLFWwindow* Window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});
}

void System::SetupGame()
{
	CubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float Vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	for (int i = 0; i < sizeof(VAOs) / sizeof(VAOs[0]); ++i)
	{
		glGenVertexArrays(1, &VAOs[i]);
		glBindVertexArray(VAOs[i]);
		glGenBuffers(1, &VBOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	Texture::SetFlipVerticallyOnLoad(true);
	SomeTexture = std::make_unique<Texture>(Texture("container.jpg", false));
	SmileyTexture = std::make_unique<Texture>(Texture("awesomeface.png", true));

	SomeTexture->Use(0);
	SmileyTexture->Use(1);

	SomeTransform = std::make_unique<Transform>(Transform());
	SomeTransform->Rotate(glm::vec3(-20.f, 0.f, 0.f));

	DefaultShader = std::make_unique<Shader>(Shader("Engine/Rendering/DefaultShaders/shader.vs", "Engine/Rendering/DefaultShaders/shader_2Textures.fs"));
	DefaultShader->Use();
	DefaultShader->SetInt("Texture1", 0);
	DefaultShader->SetInt("Texture2", 1);
	DefaultShader->SetFloat("Mix", 0.2f);

	CameraTransform = std::make_unique<Transform>(Transform());
	CameraTransform->Move(glm::vec3(0.f, 0.f, -5.f));

	CameraPosition = glm::vec3(0.f, 0.f, 3.f);
	CameraTarget = glm::vec3(0.f, 0.f, 0.f);
	CameraDirection = glm::normalize(CameraPosition - CameraTarget);
	Up = glm::vec3(0.f, 1.0f, 0.0f);
	CameraRight = glm::normalize(glm::cross(Up, CameraDirection));
	CameraUp = glm::cross(CameraDirection, CameraRight);
	CameraUp = glm::cross(CameraDirection, CameraRight);

	glEnable(GL_DEPTH_TEST);
}

void System::Run()
{
	// Temp setup while we don't have systems
	static glm::mat4 Transformation;
	static glm::mat4 View = glm::mat4(1.0f);
	static glm::mat4 Projection = glm::mat4(1.0f);
	Projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);

	static float radius = 10.0f;

	while (!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		View = glm::lookAt(glm::vec3(camX, 0.0, camZ), CameraTarget, Up);

		DefaultShader->SetMat("View", View);
		DefaultShader->SetMat("Projection", Projection);

		glBindVertexArray(VAOs[0]);

		for (int i = 0; i < CubePositions.size(); ++i)
		{
			Transformation = glm::mat4(1.f);
			Transformation = glm::translate(Transformation, CubePositions[i]);
			Transformation = glm::scale(Transformation, glm::vec3(1.f, 1.f, 1.f));
			Transformation = glm::rotate(Transformation, glm::radians(20.f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			DefaultShader->SetMat("Transform", Transformation);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
}

void System::ProcessInput(GLFWwindow* Window)
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
}
