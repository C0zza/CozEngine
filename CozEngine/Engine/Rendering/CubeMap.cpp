#include "stdafx.h"

#include "CubeMap.h"

#include "CubeMapTexture.h"
#include "ECS/ECS.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "Globes.h"
#include "Rendering/Model.h"
#include "ResourceManagement/ResourceManager.h"
#include "Shader.h"

namespace
{
	const char* CubeMapShaderPath = "Engine/Content/Shaders/CubeMapShader.casset";
}

LCubeMap::LCubeMap(const char* i_CubeMapTexture, const char* i_CubeModel)
{
	CubeMapTexture = LResourceManager::GetResource<LCubeMapTexture>(i_CubeMapTexture);
	if (!CubeMapTexture.Get())
	{
		Log(LLogLevel::ERROR, "LCubeMap::LCubeMap - Failed to load CubeMapTexture: " + std::string(i_CubeMapTexture));
	}

	CubeMapShader = LResourceManager::GetResource<LShader>(CubeMapShaderPath);
	if (!CubeMapShader.Get())
	{
		Log(LLogLevel::ERROR, "LCubeMap::LCubeMap - Failed to load CubeMapShader: " + std::string(CubeMapShaderPath));
	}

	CubeModel = LResourceManager::GetResource<LModel>(i_CubeModel);
	if (!CubeModel.Get())
	{
		Log(LLogLevel::ERROR, "LCubeMap::LCubeMap - Failed to lkoad CubeModel: " + std::string(i_CubeModel));
	}
}

void LCubeMap::Draw()
{
	if (!CubeMapShader.Get())
	{
		return;
	}

	glDepthMask(GL_FALSE);
	CubeMapShader.Get()->Use();

	LECS* ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	CCameraComponentSystem* CameraCS = dynamic_cast<CCameraComponentSystem*>(ECS->GetComponentSystemFor<CCameraComponent>());
	glm::mat4 ViewMatrix = ViewMatrix = glm::mat4(glm::mat3(CameraCS->GetViewMatrix()));
	CubeMapShader.Get()->SetMat4("View", ViewMatrix);

	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture->GetTextureID());
	CubeModel->Draw(*CubeMapShader.Get(), glm::mat4());
	glDepthMask(GL_TRUE);
}
