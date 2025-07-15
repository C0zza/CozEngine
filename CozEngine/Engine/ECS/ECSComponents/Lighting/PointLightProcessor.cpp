#include "stdafx.h"

#include "PointLightProcessor.h"

#include "DirectionalLightSubsystem.h"
#include "ECS/ECSComponents/PointLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "ECS/ECSComponents/ActiveTagComponent.h"
#include "Globes.h"

void LPointLightProcessor::Initialize()
{
	auto& Components = Config.GetComponentSet();

	Components.insert(CPointLightComponent::StaticClass());
	Components.insert(LActiveTagComponent::StaticClass());
	Components.insert(CTransformComponent::StaticClass());

	DirectionalLightSubsystem = CSystem.GetSubsystems().GetSubsystem<LDirectionalLightSubsystem>();
	assert(DirectionalLightSubsystem);

	Type = EEntityProcessorType::Logic;
}

void LPointLightProcessor::Execute(FEntityQueryResult& EntityQueryResult)
{
	// TODO: Reset point light data

	std::vector<LEntityID> PointLightIDs;
	PointLightIDs.reserve(MAX_NUM_POINT_LIGHT);

	EntityQueryResult.ForEachEntityChunk([&PointLightIDs](FEntityChunkHandle& ChunkHandle)
		{
			auto IDs = ChunkHandle.GetEntityIDs();
			for (int i = 0; i < std::min(static_cast<int>(IDs.size()), 1); ++i)
			{
				PointLightIDs.emplace_back(IDs[i]);
			}
		});

	std::vector<LEntityID> UsedIDs(PointLightIDs.begin(), PointLightIDs.begin() + std::min(static_cast<int>(PointLightIDs.size()), MAX_NUM_POINT_LIGHT));
	
}
