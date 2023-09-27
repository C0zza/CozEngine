#include "Renderer.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "Components/ModelComponent.h"
#include "Components/CameraComponent.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Object.h"

void Renderer::Init()
{
	// Utility library for OpenGL
	glfwInit();

	m_Window = std::make_shared<LWindow>();
	assert(m_Window);
	m_Window->Init();

	ProjectionMatrix = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);
}

void Renderer::Shutdown()
{
	glfwTerminate();
}

void Renderer::Tick()
{
	glClearColor(.0f, .0f, .0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Cache these?
	CCameraComponent* ActiveCamera = CCameraComponent::GetActiveCamera();
	assert(ActiveCamera);
	LShader::SetGlobalVec("ViewPos", ActiveCamera->Parent->Transform.GetPosition());
	LShader::SetGlobalMat4("View", ActiveCamera->GetViewMatrix());
	LShader::SetGlobalMat4("Projection", GetProjectionMatrix());
}

void Renderer::PostTick()
{
	assert(m_Window->m_Window);
	glfwSwapBuffers(m_Window->m_Window);
	glfwPollEvents();
}
