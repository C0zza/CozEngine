#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/Texture.h"
#include "Engine/Transform.h"

#include <vector>

// Temp input function. Should eventually setup input system.
void ProcessInput(GLFWwindow* Window)
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
}

glm::vec3 cubePositions[] = {
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

	//unsigned int Indices[] = 
	//{  // note that we start from 0!
	//	0, 1, 3,   // first triangle
	//	1, 2, 3    // second triangle
	//};  

	unsigned int VAOs[1];
	unsigned int VBOs[1];
	for (int i = 0; i < sizeof(VAOs) / sizeof(VAOs[0]); ++i)
	{
		glGenVertexArrays(1, &VAOs[i]);
		glBindVertexArray(VAOs[i]);
		glGenBuffers(1, &VBOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);*/
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);*/

	std::unique_ptr<Shader> DefaultShader(new Shader("Engine/Rendering/DefaultShaders/shader.vs", "Engine/Rendering/DefaultShaders/shader_2Textures.fs"));

	Texture::SetFlipVerticallyOnLoad(true);
	std::unique_ptr<Texture> SomeTexture(new Texture("container.jpg", false));
	std::unique_ptr<Texture> SmileyTexture(new Texture("awesomeface.png", true));

	SomeTexture->Use(0);
	SmileyTexture->Use(1);

	std::unique_ptr<Transform> SomeTransform(new Transform());
	SomeTransform->Rotate(glm::vec3(-20.f, 0.f, 0.f));

	// std::unique_ptr<Transform> ScalingTransform(new Transform());

	DefaultShader->Use();
	DefaultShader->SetInt("Texture1", 0);
	DefaultShader->SetInt("Texture2", 1);
	DefaultShader->SetFloat("Mix", 0.2f);

	glm::mat4 Transformation;

	std::unique_ptr<Transform> CameraTransform(new Transform());
	CameraTransform->Move(glm::vec3(0.f, 0.f, -5.f));

	glm::mat4 View = glm::mat4(1.0f);

	glm::mat4 Projection = glm::mat4(1.0f);
	Projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		//SomeTransform->Rotate(glm::vec3(0, 0, 0.03));
		// ScalingTransform->SetScale(glm::vec3(glm::sin(glfwGetTime()), glm::sin(glfwGetTime()), 1.0f));
		// SomeTransform->Rotate(glm::vec3(-20.f, 0.f, 0.f));

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		View = glm::mat4(1.f);
		View = glm::translate(View, CameraTransform->GetPosition());

		DefaultShader->SetMat("View", View);
		DefaultShader->SetMat("Projection", Projection);

		glBindVertexArray(VAOs[0]);

		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); ++i)
		{
			Transformation = glm::mat4(1.f);
			Transformation = glm::translate(Transformation, cubePositions[i]);
			Transformation = glm::scale(Transformation, glm::vec3(1.f, 1.f, 1.f));
			Transformation = glm::rotate(Transformation, glm::radians(20.f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			DefaultShader->SetMat("Transform", Transformation);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//for (int i = 0; i < sizeof(VAOs) / sizeof(VAOs[0]); ++i)
		//{
		//	glBindVertexArray(VAOs[i]);
		//	for (int j = 0; j < 9; ++j)
		//	{
		//		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//		glDrawArrays(GL_TRIANGLES, 0, 36);
		//	}
		//}

		/*Transformation = glm::mat4(1.f);
		Transformation = glm::translate(Transformation, ScalingTransform->GetPosition());
		Transformation = glm::scale(Transformation, ScalingTransform->GetScale());
		DefaultShader->SetMat("Transform", Transformation);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	// glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}