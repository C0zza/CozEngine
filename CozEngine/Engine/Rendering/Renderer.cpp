#include "Renderer.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DirectionalLightSubsystem.h"
#include "Editor/DrawModeSubsystem.h"
#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "ECS/ECSComponents/DirectionalLightComponent.h"
#include "ECS/ECSComponents/PointLightComponent.h"
#include "ECS/ECSComponents/SpotLightComponent.h"
#include "FrameBuffer.h"
#include "Globes.h"
#include "Rendering/RendererInfo.h"
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

	RendererInfo = CSystem.GetSubsystems().AddSubsystem<LRendererInfo>();
	RendererInfo->UpdateProjectionMatrix(1280, 720);

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

	CSystem.GetSubsystems().AddSubsystem<LDrawModeSubsystem>();
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

	if (!RendererInfo || !RendererInfo->CanRender())
	{
		return;
	}

	LDirectionalLightSubsystem* DirectionalLightSubsystem = CSystem.GetSubsystems().GetSubsystem<LDirectionalLightSubsystem>();
	DirectionalLightSubsystem->UpdatePointLightData();
	DirectionalLightSubsystem->UpdateSpotLightData();
	DirectionalLightSubsystem->UpdateDirectionalLightData();
	
	glBindBuffer(GL_UNIFORM_BUFFER, MatricesUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &RendererInfo->GetProjectionMatrix());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &RendererInfo->GetViewMatrix());
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &RendererInfo->GetViewPos());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
