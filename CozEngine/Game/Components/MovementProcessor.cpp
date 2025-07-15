#include "stdafx.h"

#include "MovementProcessor.h"

#include "Components/MovementComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"

void LMovementProcessor::Initialize()
{
	auto& Components = Config.GetComponentSet();

	Components.insert(CTransformComponent::StaticClass());
	Components.insert(CMovementComponent::StaticClass());

	Type = EEntityProcessorType::Logic;
}

void LMovementProcessor::Execute(FEntityQueryResult& EntityQueryResult)
{
	EntityQueryResult.ForEachEntityChunk([](FEntityChunkHandle& ChunkHandle)
		{
			auto IDs = ChunkHandle.GetEntityIDs();

			auto Transforms = ChunkHandle.GetComponentSpan<CTransformComponent>();
			auto Movements = ChunkHandle.GetComponentSpan<CMovementComponent>();

			for (int i = 0; i < IDs.size(); ++i)
			{
				auto& Transform = Transforms[i];
				auto& Movement = Movements[i];

				const bool bUpdateMovement = glm::length(Movement.Movement) > 0;
				const bool bUpdateRotation = glm::length(Movement.Rotation) > 0;

				if (bUpdateMovement)
				{
					Transform.MoveRelative(glm::normalize(Movement.Movement) * Movement.Speed);
				}

				if (bUpdateRotation)
				{
					Transform.Rotate(Movement.Rotation * Movement.Sensitivity);
				}
			}
		});
}
