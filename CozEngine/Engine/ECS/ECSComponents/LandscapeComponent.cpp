#include "stdafx.h"

#include "LandscapeComponent.h"

#include "ECS/ECS.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"
#include "Rendering/Mesh.h"
#include "ResourceManagement/ResourceManager.h"

#if defined(COZ_EDITOR)
#include "Editor/ImGuiPropertyDrawHelpers.h"
#endif

namespace
{
	const char* LandscapeShaderPath = "Engine/Content/Shaders/LandscapeShaders/LandscapeShader.casset";
#if defined(COZ_EDITOR)
	const char* LandscapeEntityBufferShaderPath = "Engine/Content/Shaders/LandscapeShaders/LandscapeShader_EntityBuffer.casset";
#endif
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

	const glm::vec3 LandscapeSize(Width, Height, Length);
	LandscapeShader->SetVec3("LandscapeSize", LandscapeSize);

	LandscapeShader.Get()->SetInt("HeightMap", 0);
	LandscapeShader.Get()->SetInt("GroundTexture", 1);
	LandscapeShader.Get()->SetInt("WallTexture", 2);

#if defined(COZ_EDITOR)
	ResourceManager->GetResource<LShader>(LandscapeEntityBufferShaderPath, LandscapeEntityBufferShader);
	if (!LandscapeEntityBufferShader.Get())
	{
		Log(LLogLevel::ERROR, std::string("CLandscapeComponentSystem::CLandscapeComponentSystem - Failed to load landscape entity buffer shader ")
			+ LandscapeEntityBufferShaderPath + ". Unable to set landscape dimensions.");
		return;
	}

	LandscapeEntityBufferShader->Use();
	LandscapeEntityBufferShader->SetVec3("LandscapeSize", LandscapeSize);
#endif
}

void CLandscapeComponentSystem::PreRun()
{
	assert(LandscapeShader.Get());
	LandscapeShader->Use();
}

void CLandscapeComponentSystem::RunComponent(CLandscapeComponent& Component)
{
	CTransformComponent* Transform = ECS->GetComponent<CTransformComponent>(Component.EntityID);
	if (!Transform)
	{
		Log(LLogLevel::ERROR, "CLandscapeComponentSystem::RunComponent - Failed to get transform component.");
		return;
	}

	if (!Component.LandscapeMaterial.Get())
	{
		Log(LLogLevel::WARNING, "CLandscapeComponentSystem::RunComponent - Invalid LandscapeMaterial.");
		return;
	}

#if defined(COZ_EDITOR)
	Component.LandscapeMaterial->EntityID = Component.EntityID;
#endif

	const LShader* ActiveShader = Component.LandscapeMaterial->Use();
	if (!ActiveShader)
	{
		return;
	}

	assert(Component.LandscapeMaterial->HeightMap->GetWidth() == Component.LandscapeMaterial->HeightMap->GetHeight());
	LandscapeShader->SetFloat("HeightMapSize", Component.LandscapeMaterial->HeightMap->GetWidth());

	LandscapeMesh->Draw(*ActiveShader, Transform->GetUpdatedTransformationMatrix());
}

#if defined(COZ_EDITOR)
void CLandscapeComponentSystem::DrawImGuiComponent(CLandscapeComponent& Component)
{
	LImGuiPropertyDrawHelpers::DrawProperty("Landscape Material", Component.LandscapeMaterial);
}
#endif

void CLandscapeComponentSystem::GetSerializedComponent(const CLandscapeComponent& Component, nlohmann::json& J) const
{
	J["LandscapeMaterial"] = Component.LandscapeMaterial;
}

void CLandscapeComponentSystem::DeserializeComponent(CLandscapeComponent& Component, const nlohmann::json& J)
{
	Component.LandscapeMaterial = J["LandscapeMaterial"];
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

CLandscapeComponent::CLandscapeComponent(const std::string& i_LandscapeMaterial)
{
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	if (!ResourceManager)
	{
		Log(LLogLevel::ERROR, "CLandscapeComponent::CLandscapeComponent - Failed to get ResourceManager.");
		return;
	}

	ResourceManager->GetResource<LLandscapeMaterial>(i_LandscapeMaterial, LandscapeMaterial);
}
