#include "stdafx.h"

#include "SpotLightProcessor.h"

#include "DirectionalLightSubsystem.h"
#include "ECS/ECSComponents/ActiveTagComponent.h"
#include "ECS/ECSComponents/SpotLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"

void LSpotLightProcessor::Initialize()
{
	auto& Components = Config.GetComponentSet();

	Components.insert(LActiveTagComponent::StaticClass());
	Components.insert(CSpotLightComponent::StaticClass());
	Components.insert(CTransformComponent::StaticClass());

	DirectionalLightSubsystem = CSystem.GetSubsystems().GetSubsystem<LDirectionalLightSubsystem>();
	assert(DirectionalLightSubsystem);

	Type = EEntityProcessorType::Logic;
}

void LSpotLightProcessor::Execute(FEntityQueryResult& EntityQueryResult)
{
	DirectionalLightSubsystem->SetSpotLightEntityIDs({});

	std::vector<LEntityID> SpotLightIDs;
	SpotLightIDs.reserve(MAX_NUM_SPOT_LIGHTS);

	EntityQueryResult.ForEachEntityChunk([&SpotLightIDs](FEntityChunkHandle& ChunkHandle)
		{
			auto IDs = ChunkHandle.GetEntityIDs();
			for (int i = 0; i < std::min(static_cast<int>(IDs.size()), MAX_NUM_SPOT_LIGHTS); ++i)
			{
				SpotLightIDs.emplace_back(IDs[i]);
			}
		});

	std::vector<LEntityID> UsedIDs(SpotLightIDs.begin(), SpotLightIDs.begin() + std::min(static_cast<int>(SpotLightIDs.size()), MAX_NUM_SPOT_LIGHTS));
	DirectionalLightSubsystem->SetSpotLightEntityIDs(UsedIDs);
}
