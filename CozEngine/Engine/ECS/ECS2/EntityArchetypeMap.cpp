#include "stdafx.h"

#include "EntityArchetypeMap.h"

#include "ECS/ECSDefinitions.h"
#include "Globes.h"
#include "Misc/Logging.h"
#include "Misc/TypeIdGenerator.h"

const FEntityArchetypeMapping* LEntityArchetypeMap::GetMapping(const LEntityID EntityID) const
{
	if (EntityArchetypeMappings.contains(EntityID))
	{
		return &EntityArchetypeMappings.at(EntityID);
	}

	return nullptr;
}

FEntityArchetypeMapping& LEntityArchetypeMap::GetMapping(const LEntityID EntityID)
{
	return EntityArchetypeMappings.at(EntityID);
}

FEntityArchetypeMapping& LEntityArchetypeMap::RegisterEntity(LArchetype* Archetype, std::size_t ChunkIndex, std::size_t ElementIndex, LEntityID& OutEntityID)
{
	LTypeInstanceIdGenerator* TypeInstanceIdGenerator = CSystem.GetSubsystems().GetSubsystem<LTypeInstanceIdGenerator>();

	OutEntityID = TypeInstanceIdGenerator->GetNewID<LEntityID>();

	assert(!EntityArchetypeMappings.contains(OutEntityID));
	EntityArchetypeMappings.emplace(OutEntityID, FEntityArchetypeMapping(Archetype, ChunkIndex, ElementIndex));
	return EntityArchetypeMappings.at(OutEntityID);
}

void LEntityArchetypeMap::UnregisterEntity(const LEntityID EntityID)
{
	if (!EntityArchetypeMappings.contains(EntityID))
	{
		Log(LLogLevel::ERROR, "LEntityArchetypeMap::UnregisterEntity - EntityID: " + std::to_string(EntityID) + " not found.");
		return;
	}

	EntityArchetypeMappings.erase(EntityID);

	LTypeInstanceIdGenerator* TypeInstanceIdGenerator = CSystem.GetSubsystems().GetSubsystem<LTypeInstanceIdGenerator>();
	TypeInstanceIdGenerator->UnregisterID<LEntityID>(EntityID);
}
