#include "Renderer.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "ECS/ECSComponents/DirectionalLightComponent.h"
#include "ECS/ECSComponents/PointLightComponent.h"
#include "ECS/ECSComponents/SpotLightComponent.h"

#include "FrameBuffer.h"
#include "Globes.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

void LRenderer::BindCustomFrameBuffer()
{
	if (CustomFrameBuffer)
	{
		CustomFrameBuffer->Bind();
		glClearColor(.0f, .0f, .0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void LRenderer::UnbindCustomFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LRenderer::Initialize()
{
	// Utility library for OpenGL
	glfwInit();

	m_Window = new LWindow();
	assert(m_Window);
	m_Window->Init();

	ProjectionMatrix = glm::perspective(glm::radians(60.0f), 1280.f / 720.f, 0.1f, 100.f);

	LTexture::SetFlipVerticallyOnLoad(true);

	ECS = CSystem.GetSubsystems().GetSubsystem<LECS>(true);
}

void LRenderer::Deinitialize()
{
	glfwTerminate();
}

LRenderer::~LRenderer()
{
	if (m_Window)
	{
		delete m_Window;
		m_Window = nullptr;
	}
}

bool LRenderer::HasRequiredSubsystems() const
{
	return CSystem.GetSubsystems().GetSubsystem<LECS>(true);
}

void LRenderer::Update()
{
	CPointLightComponent::UpdatePointLights();
	CSpotLightComponent::UpdateSpotLights();

	LComponentSystemBase* ComponentSystem = ECS->GetComponentSystemFor<CDirectionalLightComponent>();
	CDirectionalLightComponentSystem* DirectionalLightCS = dynamic_cast<CDirectionalLightComponentSystem*>(ComponentSystem);
	if (DirectionalLightCS)
	{
		DirectionalLightCS->UpdateDirectionalLight();
	}

	glClearColor(.0f, .0f, .0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO update automatically by delegate. I.e, setup delegates
	assert(ECS);
	CCameraComponent* ActiveCamera = ECS->GetComponent<CCameraComponent>(CCameraComponent::GetActiveCameraEntityID());

	LShader::SetGlobalVec("ViewPos", ActiveCamera->GetViewPos());
	LShader::SetGlobalMat4("View", ActiveCamera->GetViewMatrix());
	LShader::SetGlobalMat4("Projection", GetProjectionMatrix());
}

void LRenderer::Swap()
{
	assert(m_Window->m_Window);
	glfwSwapBuffers(m_Window->m_Window);
	glfwPollEvents();
}

void LRenderer::GetShouldWindowClose(bool& bClose)
{
	assert(m_Window);
	bClose = m_Window->ShouldClose();
}
