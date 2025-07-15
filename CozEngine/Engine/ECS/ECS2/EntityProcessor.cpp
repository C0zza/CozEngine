#include "stdafx.h"

#include "EntityProcessor.h"

FEntityQueryResult::FEntityQueryResult(std::vector<LArchetype*> InArchetypes)
	: Archetypes{ InArchetypes }
{
}

void FEntityQueryResult::ForEachEntityChunk(std::function<void(FEntityChunkHandle& ChunkHandle)> ForEachEntityChunkFunc)
{
	for (LArchetype* Archetype : Archetypes)
	{
		Archetype->ForEachEntityChunk(ForEachEntityChunkFunc);
	}
}
