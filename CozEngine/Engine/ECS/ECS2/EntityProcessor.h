#pragma once

#include "ECS/ECS2/Archetype.h"
#include "ECS/ECS2/ArchetypeConfig.h"
#include "ECS/ECS2/EntityProcessorType.h"
#include "Reflection/Reflection.h"

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

	virtual void ForEachEntityChunk(FEntityChunkHandle& EntityChunkHandle) = 0;

protected:
	LArchetypeConfig Config;
	EEntityProcessorType Type = EEntityProcessorType::Invalid;
};