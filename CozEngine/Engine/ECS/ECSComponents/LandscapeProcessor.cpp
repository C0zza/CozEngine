#include "stdafx.h"

#include "LandscapeProcessor.h"

#include "ECS/ECSComponents/LandscapeComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "ResourceManagement/ResourceManager.h"

namespace
{
	const char* LandscapeShaderPath = "Engine/Content/Shaders/LandscapeShaders/LandscapeShader.casset";
#if defined(COZ_EDITOR)
	const char* LandscapeEntityBufferShaderPath = "Engine/Content/Shaders/LandscapeShaders/LandscapeShader_EntityBuffer.casset";
#endif
}

LLandscapeProcessor::LLandscapeProcessor() = default;
LLandscapeProcessor::~LLandscapeProcessor() = default;

void LLandscapeProcessor::Initialize()
{
	std::set<TSubclassOf<LECSComponent>>& Components = Config.GetComponentSet();

	Components.insert(CTransformComponent::StaticClass());
	Components.insert(CLandscapeComponent::StaticClass());

	Type = EEntityProcessorType::Render;

	GenerateMesh();

	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();

	ResourceManager->GetResource<LShader>(LandscapeShaderPath, LandscapeShader);
	if (!LandscapeShader.Get())
	{
		Log(LLogLevel::ERROR, std::string("LLandscapeProcessor::Initialize - Failed to load landscape shader ")
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

void LLandscapeProcessor::Execute(FEntityQueryResult& EntityQueryResult)
{
	assert(LandscapeShader.Get());
	LandscapeShader->Use();

	EntityQueryResult.ForEachEntityChunk([this](FEntityChunkHandle& ChunkHandle)
		{
			auto IDs = ChunkHandle.GetEntityIDs();

			auto Transforms = ChunkHandle.GetComponentSpan<CTransformComponent>();
			auto Landscapes = ChunkHandle.GetComponentSpan<CLandscapeComponent>();

			for (int i = 0; i < IDs.size(); ++i)
			{
				const auto ID = IDs[i];
				auto& Transform = Transforms[i];
				auto& Landscape = Landscapes[i];

				if (!Landscape.LandscapeMaterial.Get())
				{
					continue;
				}

#if defined(COZ_EDITOR)
				Landscape.LandscapeMaterial->EntityID = ID;
#endif

				const LShader* ActiveShader = Landscape.LandscapeMaterial->Use();
				if (!ActiveShader)
				{
					continue;
				}

				assert(Landscape.LandscapeMaterial->HeightMap->GetWidth() == Landscape.LandscapeMaterial->HeightMap->GetHeight());
				LandscapeShader->SetFloat("HeightMapSize", Landscape.LandscapeMaterial->HeightMap->GetWidth());

				LandscapeMesh->Draw(*ActiveShader, Transform.GetUpdatedTransformationMatrix());
			}
		});
}

void LLandscapeProcessor::GenerateMesh()
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
