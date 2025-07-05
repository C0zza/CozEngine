#pragma once

#include "Subsystem.h"

class LArchetype;

struct FEntityArchetypeMapping
{
	LArchetype* Archetype = nullptr;
	std::size_t ChunkIndex = -1;
	std::size_t ElementIndex = -1;
};

class LEntityArchetypeMap : public LSubsystem
{
	friend class LArchetype;
	friend class LArchetypeManager;
public:
	const FEntityArchetypeMapping* GetMapping(const LEntityID EntityID) const;

private:
	FEntityArchetypeMapping& GetMapping(const LEntityID EntityID);

	FEntityArchetypeMapping& RegisterEntity(LArchetype* Archetype, std::size_t ChunkIndex, std::size_t ElementIndex, LEntityID& OutEntityID);
	void UnregisterEntity(const LEntityID EntityID);

	std::unordered_map<LEntityID, FEntityArchetypeMapping> EntityArchetypeMappings;
};

