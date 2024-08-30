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

void LRenderer::BindFrameBuffer(const LFrameBuffer* Buffer)
{
	Buffer->Bind();
}

void LRenderer::BindDefaultFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LRenderer::ClearFrameBuffer(const float& R, const float& G, const float& B, const float& A, GLbitfield ClearFlags)
{
	glClearColor(R, G, B, A);
	glClear(ClearFlags);
}

void LRenderer::Initialize()
{
	// Utility library for OpenGL
	glfwInit();

	m_Window = new LWindow();
	assert(m_Window);
	m_Window->Init();

	ProjectionMatrix = glm::perspective(glm::radians(60.0f), 1280.f / 720.f, 0.1f, 500.f);

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
	assert(ECS);

	LComponentSystemBase* ComponentSystem = ECS->GetComponentSystemFor<CPointLightComponent>();
	if (CPointLightComponentSystem* PointLightCS = dynamic_cast<CPointLightComponentSystem*>(ComponentSystem))
	{
		PointLightCS->UpdatePointLights();
	}

	ComponentSystem = ECS->GetComponentSystemFor<CSpotLightComponent>();
	if (CSpotLightComponentSystem* SpotLightCS = dynamic_cast<CSpotLightComponentSystem*>(ComponentSystem))
	{
		SpotLightCS->UpdateSpotLights();
	}

	ComponentSystem = ECS->GetComponentSystemFor<CDirectionalLightComponent>();
	if (CDirectionalLightComponentSystem* DirectionalLightCS = dynamic_cast<CDirectionalLightComponentSystem*>(ComponentSystem))
	{
		DirectionalLightCS->UpdateDirectionalLight();
	}

	// TODO update automatically by delegate. I.e, setup delegates
	ComponentSystem = ECS->GetComponentSystemFor<CCameraComponent>();
	CCameraComponentSystem* CameraCS = dynamic_cast<CCameraComponentSystem*>(ComponentSystem);
	
	if (CameraCS)
	{
		LShader::SetGlobalVec("ViewPos", CameraCS->GetViewPos());
		LShader::SetGlobalMat4("View", CameraCS->GetViewMatrix());
		LShader::SetGlobalMat4("Projection", GetProjectionMatrix());
	}
	else
	{
		Log(LLogLevel::WARNING, "Failed to update view shader data");
	}
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
