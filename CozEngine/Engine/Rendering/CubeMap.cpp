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
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	ResourceManager->GetResource<LCubeMapTexture>(i_CubeMapTexture, CubeMapTexture);
	if (!CubeMapTexture.Get())
	{
		Log(LLogLevel::ERROR, "LCubeMap::LCubeMap - Failed to load CubeMapTexture: " + std::string(i_CubeMapTexture));
	}

	ResourceManager->GetResource<LShader>(CubeMapShaderPath, CubeMapShader);
	if (!CubeMapShader.Get())
	{
		Log(LLogLevel::ERROR, "LCubeMap::LCubeMap - Failed to load CubeMapShader: " + std::string(CubeMapShaderPath));
	}

	ResourceManager->GetResource<LModel>(i_CubeModel, CubeModel);
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
	// Casting to mat3 removes the translation data of the view matrix so the cube map position
	// is static relative to the camera.
	glm::mat4 ViewMatrix = glm::mat4(glm::mat3(CameraCS->GetViewMatrix()));
	CubeMapShader.Get()->SetMat4("CubeMapView", ViewMatrix);

	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture->GetTextureID());
	CubeModel->Draw(*CubeMapShader.Get(), glm::mat4());
	glDepthMask(GL_TRUE);
}
