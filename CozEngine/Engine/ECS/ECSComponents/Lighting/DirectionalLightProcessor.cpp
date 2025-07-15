#include "stdafx.h"

#include "DirectionalLightProcessor.h"

#include "DirectionalLightSubsystem.h"
#include "ECS/ECSComponents/DirectionalLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "ECS/ECSComponents/ActiveTagComponent.h"
#include "Globes.h"

void LDirectionalLightProcessor::Initialize()
{
	auto& Components = Config.GetComponentSet();

	Components.insert(CDirectionalLightComponent::StaticClass());
	Components.insert(LActiveTagComponent::StaticClass());
	Components.insert(CTransformComponent::StaticClass());

	DirectionalLightSubsystem = CSystem.GetSubsystems().GetSubsystem<LDirectionalLightSubsystem>(true);
	assert(DirectionalLightSubsystem);

	Type = EEntityProcessorType::Logic;
}

void LDirectionalLightProcessor::Execute(FEntityQueryResult& EntityQueryResult)
{
	DirectionalLightSubsystem->SetDirectionalLightEntityID(0);

	EntityQueryResult.ForEachEntityChunk([this](FEntityChunkHandle& ChunkHandle)
		{
			auto IDs = ChunkHandle.GetEntityIDs();

			const auto EntityCount = ChunkHandle.GetEntityCount();

			for (int i = 0; i < std::min(EntityCount, 1); ++i)
			{
				DirectionalLightSubsystem->SetDirectionalLightEntityID(IDs[i]);
			}
		});
}
