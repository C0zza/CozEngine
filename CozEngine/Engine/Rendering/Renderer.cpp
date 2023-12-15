#include "Renderer.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "Components/ModelComponent.h"
#include "Components/CameraComponent.h"
#include "Components/Lighting/DirectionalLightComponent.h"
#include "Components/Lighting/PointLightComponent.h"
#include "ECS/ECS.h"
#include "ECS/ECSComponents/ECSCameraComponent.h"
#include "ECS/ECSComponents/ECSPointLightComponent.h"
#include "ECS/ECSComponents/ECSSpotLightComponent.h"
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
	CECSPointLightComponent::UpdatePointLights();
	CECSSpotLightComponent::UpdateSpotLights();
	CDirectionalLightComponent::UpdateDirectionalLight();

	glClearColor(.0f, .0f, .0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CECSCameraComponent* ActiveCamera = LECS::Get()->GetComponent<CECSCameraComponent>(CECSCameraComponent::GetActiveCameraEntityID());

	LShader::SetGlobalVec("ViewPos", ActiveCamera->GetViewPos());
	LShader::SetGlobalMat4("View", ActiveCamera->GetViewMatrix());
	LShader::SetGlobalMat4("Projection", GetProjectionMatrix());
}

void Renderer::PostTick()
{
	assert(m_Window->m_Window);
	glfwSwapBuffers(m_Window->m_Window);
	glfwPollEvents();
}
