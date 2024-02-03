#include "Renderer.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "ECS/ECSComponents/DirectionalLightComponent.h"
#include "ECS/ECSComponents/PointLightComponent.h"
#include "ECS/ECSComponents/SpotLightComponent.h"

#if defined(COZ_EDITOR)
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#endif

#include "Shader.h"
#include "Window.h"

void Renderer::Init()
{
	// Utility library for OpenGL
	glfwInit();

	m_Window = std::make_shared<LWindow>();
	assert(m_Window);
	m_Window->Init();

	ProjectionMatrix = glm::perspective(glm::radians(60.0f), 1280.f / 720.f, 0.1f, 100.f);
}

void Renderer::Shutdown()
{
	glfwTerminate();
}

void Renderer::Tick()
{
	CPointLightComponent::UpdatePointLights();
	CSpotLightComponent::UpdateSpotLights();
	CDirectionalLightComponent::UpdateDirectionalLight();

	glClearColor(.0f, .0f, .0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CCameraComponent* ActiveCamera = LECS::Get()->GetComponent<CCameraComponent>(CCameraComponent::GetActiveCameraEntityID());

	LShader::SetGlobalVec("ViewPos", ActiveCamera->GetViewPos());
	LShader::SetGlobalMat4("View", ActiveCamera->GetViewMatrix());
	LShader::SetGlobalMat4("Projection", GetProjectionMatrix());
}

void Renderer::PostTick()
{
#if defined(COZ_EDITOR)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("My name is window, ImGui window");
	ImGui::Text("Hello there!");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

	assert(m_Window->m_Window);
	glfwSwapBuffers(m_Window->m_Window);
	glfwPollEvents();
}
