#pragma once

#include <array>
#include <queue>

#include "ECSDefinitions.h"

class LEntityManager
{
public:
	LEntityManager();

	LEntity CreateEntity();
	void DestroyEntity(LEntity Entity);

	void SetSignature(LEntity Entity, LSignature Signature);
	LSignature GetSignature(LEntity Entity);

private:
	// Unused entity Ids
	std::queue<LEntity> AvailableEntities;

	// Signatures[EntityID] = Entities signature
	std::array<LSignature, MAX_ENTITIES> Signatures{};

	std::size_t LivingEntityCount = 0;
};

