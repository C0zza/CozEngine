#pragma once

#include <functional>

#include "ECS/ECS2/Archetype.h"
#include "ECS/ECS2/ArchetypeConfig.h"
#include "ECS/ECS2/EntityProcessorType.h"
#include "Reflection/Reflection.h"

struct FEntityQueryResult
{
public:
	FEntityQueryResult(std::vector<LArchetype*> InArchetypes);

	void ForEachEntityChunk(std::function<void(FEntityChunkHandle& ChunkHandle)>);

private:
	std::vector<LArchetype*> Archetypes;
};

// TODO: Rename class. We're not technically processing archetypes.
// Just specified combinations of components, which could include
// multiple archetypes.

REFL_CLASS(Abstract)
class LEntityProcessor
{
	REFL_GENERATED_BODY(LEntityProcessor)
public:
	virtual void Initialize() {}

	virtual bool ShouldAutoRegister() const { return true; }

	const LArchetypeConfig& GetConfig() const { return Config; }
	EEntityProcessorType GetType() const { return Type; }

	virtual void Execute(FEntityQueryResult& EntityQueryResult) = 0;

protected:
	LArchetypeConfig Config;
	EEntityProcessorType Type = EEntityProcessorType::Invalid;
};