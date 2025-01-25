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

void LRenderer::RegisterMatricesUBOToShader(const unsigned int ShaderID)
{
	unsigned int UniformBlockIndex = glGetUniformBlockIndex(ShaderID, "Matrices");
	glUniformBlockBinding(ShaderID, UniformBlockIndex, 0);
}

void LRenderer::ReigsterLightingUBOToShader(const unsigned int ShaderID)
{
	unsigned int UniformBlockIndex = glGetUniformBlockIndex(ShaderID, "Lights");
	glUniformBlockBinding(ShaderID, UniformBlockIndex, 1);
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

	glGenBuffers(1, &MatricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, MatricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, MatricesUBO, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));

	glGenBuffers(1, &LightingUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, LightingUBO);
	glBufferData(GL_UNIFORM_BUFFER, 856, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, LightingUBO, 0, 856);
}

void LRenderer::Deinitialize()
{
	glDeleteBuffers(1, &LightingUBO);
	glDeleteBuffers(1, &MatricesUBO);
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
		glBindBuffer(GL_UNIFORM_BUFFER, MatricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &GetProjectionMatrix());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &CameraCS->GetViewMatrix());
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &CameraCS->GetViewPos());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
