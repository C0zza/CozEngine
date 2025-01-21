#include "stdafx.h"

#include "LandscapeComponent.h"

#include "ECS/ECS.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"
#include "Rendering/Mesh.h"
#include "ResourceManagement/ResourceManager.h"

namespace
{
	const char* LandscapeShaderPath = "Engine/Content/Shaders/LandscapeShader.casset";
}

CLandscapeComponentSystem::CLandscapeComponentSystem()
	: Width{ 512 }, Height{ 100 }, Length{ 512 }
{
	GenerateMesh();
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	ResourceManager->GetResource<LShader>(LandscapeShaderPath, LandscapeShader);

	if (!LandscapeShader.Get())
	{
		Log(LLogLevel::ERROR, std::string("CLandscapeComponentSystem::CLandscapeComponentSystem - Failed to load landscape shader ") 
			+ LandscapeShaderPath + ". Unable to set landscape dimensions.");
		return;
	}

	LandscapeShader->Use();
	LandscapeShader->SetVec3("LandscapeSize", glm::vec3(Width, Height, Length));
	LandscapeShader.Get()->SetInt("HeightMap", 0);
	LandscapeShader.Get()->SetInt("GroundTexture", 1);
	LandscapeShader.Get()->SetInt("WallTexture", 2);
}

void CLandscapeComponentSystem::PreRun()
{
	assert(LandscapeShader.Get());
	LandscapeShader->Use();
	LandscapeShader->SetInt("HeightMap", 0);
}

void CLandscapeComponentSystem::RunComponent(CLandscapeComponent& Component)
{
	CTransformComponent* Transform = ECS->GetComponent<CTransformComponent>(Component.EntityID);
	if (!Transform)
	{
		Log(LLogLevel::ERROR, "CLandscapeComponentSystem::RunComponent - Failed to get transform component.");
		return;
	}

	if (!Component.HeightMap.Get())
	{
		Log(LLogLevel::ERROR, "CLandscapeComponentSystem::RunComponent - Height map is invalid. Unable to render.");
		return;
	}

	assert(Component.HeightMap->GetWidth() == Component.HeightMap->GetHeight());
	LandscapeShader->SetFloat("HeightMapSize", 512);

	Component.HeightMap->Use(0);
	Component.GroundTexture->Use(1);
	Component.WallTexture->Use(2);

	LandscapeMesh->Draw(*LandscapeShader.Get(), Transform->GetUpdatedTransformationMatrix());
}

void CLandscapeComponentSystem::GenerateMesh()
{
	using namespace std;

	vector<Vertex> Vertices;
	Vertices.resize(Width * Length);

	const int ww = Width - 1;
	const int hh = Length - 1;

	vector<unsigned int> Indices;
	Indices.resize((ww) * (hh) * 6);

	int i = 0;
	for (int x = 0; x < Width; ++x)
	{
		for (int y = 0; y < Length; ++y)
		{
			const int VerticesIndex = y * Width + x;
			Vertices[VerticesIndex].Position = glm::vec3((float)x / (float)ww, 0, (float)y / (float)hh);
			Vertices[VerticesIndex].Normal = glm::vec3(0.f);
			Vertices[VerticesIndex].TexCoords = glm::vec2((float)x / (float)ww, (float)y / (float)hh);

			if (x < ww && y < hh)
			{
				Indices[i] = (y + 1) * Width + x;
				Indices[i + 1] = y * Width + x + 1;
				Indices[i + 2] = y * Width + x;

				Indices[i + 3] = y * Width + x + 1;
				Indices[i + 4] = (y + 1) * Width + x;
				Indices[i + 5] = (y + 1) * Width + (x + 1);
				i += 6;
			}
		}
	}

	LandscapeMesh = make_unique<LMesh>(forward<vector<Vertex>>(Vertices), forward<vector<unsigned int>>(Indices));
}

CLandscapeComponent::CLandscapeComponent(const std::string& i_HeightMap, const std::string& i_GroundTexture, const std::string& i_WallTexture)
{
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	ResourceManager->GetResource<LTexture>(i_HeightMap, HeightMap);
	if (!HeightMap.Get())
	{
		Log(LLogLevel::ERROR, "CLandscapeComponent::CLandscapeComponent - Failed to load height map: " + i_HeightMap);
	}

	ResourceManager->GetResource<LTexture>(i_GroundTexture, GroundTexture);
	if (!GroundTexture.Get())
	{
		Log(LLogLevel::ERROR, "CLandscapeComponent::CLandscapeComponent - Failed to load ground texture: " + i_GroundTexture);
	}

	ResourceManager->GetResource<LTexture>(i_WallTexture, WallTexture);
	if (!WallTexture.Get())
	{
		Log(LLogLevel::ERROR, "CLandscapeComponent::CLandscapeComponent - Failed to load wall texture: " + i_WallTexture);
	}
}
