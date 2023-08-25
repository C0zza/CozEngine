#include "System.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Transform.h"

#include <iostream>

System::System()
{
}

System::~System()
{
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	// glDeleteBuffers(1, &EBO);

	//glfwTerminate();
}

void System::Init()
{
	m_Renderer.Init();
	m_InputManager.Init(m_Renderer.GetWindow());
}

void System::Shutdown()
{
	m_Renderer.Shutdown();
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

	DefaultShader = std::make_unique<Shader>(Shader("Engine/Rendering/DefaultShaders/shader.vs", "Engine/Rendering/DefaultShaders/shader_2Textures.fs"));
	DefaultShader->Use();
	DefaultShader->SetInt("Texture1", 0);
	DefaultShader->SetInt("Texture2", 1);
	DefaultShader->SetFloat("Mix", 0.2f);

	LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	DefaultShader->SetVec3("LightColor", LightColor);

	SomeCamera = std::make_shared<Camera>();
	SomeCamera->CameraTransform->Move(glm::vec3(0.f, 0.f, 6.f));
	m_Renderer.SetActiveCamera(SomeCamera);
}

void System::Run()
{
	// Temp setup while we don't have systems
	static glm::mat4 Transformation;

	static float radius = 10.0f;

	assert(m_Renderer.GetWindow());
	while (!m_Renderer.GetWindow()->ShouldClose())
	{
		m_InputManager.ProcessInput();

		m_Renderer.Tick();

		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		SomeCamera->CameraTransform->SetPosition(glm::vec3(camX, 0.f, camZ));

		DefaultShader->SetMat("View", m_Renderer.GetViewMatrix());
		DefaultShader->SetMat("Projection", m_Renderer.GetProjectionMatrix());

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

		m_Renderer.PostTick();
	}
}
