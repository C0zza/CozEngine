#pragma once

#include <array>
#include <cassert>
#include <unordered_map>

#include "ECSDefinitions.h"

class IComponentPool
{
public:
	virtual ~IComponentPool() = default;
	virtual void EntityDestroyed(LEntity Entity) = 0;
};

template<typename T>
class LComponentPool : public IComponentPool
{
public:
	void InsertData(LEntity Entity, T Component)
	{
		assert(EntityToIndexMap.find(Entity) == EntityToIndexMap.end());

		std::size_t NewIndex = Size;
		EntityToIndexMap[Entity] = NewIndex;
		IndexToEntityMap[NewIndex] = Entity;
		Pool[NewIndex] = Component;
		++Size;
	}

	void RemoveData(LEntity Entity)
	{
		assert(EntityToIndexMap.find(Entity) != EntityToIndexMap.end());

		// Copy first unused element into removed spot
		std::size_t IndexOfRemovedEntity = EntityToIndexMap[Entity];
		std::size_t IndexOfLastElement = Size - 1;
		Pool[IndexOfRemovedEntity] = Pool[IndexOfLastElement];

		LEntity EntityOfLastElement = IndexToEntityMap[IndexOfLastElement];
		EntityToIndexMap[EntityOfLastElement] = IndexOfRemovedEntity;
		IndexToEntityMap[IndexOfRemovedEntity] = EntityOfLastElement;

		EntityToIndexMap.erase(Entity);
		IndexToEntityMap.erase(IndexOfLastElement);

		--Size;
	}

	T& GetData(LEntity Entity)
	{
		assert(EntityToIndexMap.find(Entity) != EntityToIndexMap.end());
		return Pool[EntityToIndexMap[Entity]];
	}

	void EntityDestroyed(LEntity Entity) override
	{
		if (EntityToIndexMap.find(Entity) != EntityToIndexMap.end())
		{
			RemoveData(Entity);
		}
	}

private:
	std::array<T, MAX_ENTITIES> Pool;

	// Used to keep track of entities vs their indexes
	std::unordered_map<LEntity, std::size_t> EntityToIndexMap;
	std::unordered_map<std::size_t, LEntity> IndexToEntityMap;

	std::size_t Size;
};
