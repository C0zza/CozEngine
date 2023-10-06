#include "EntityManager.h"

#include <cassert>

LEntityManager::LEntityManager()
{
	for (LEntity Entity = 0; Entity < MAX_ENTITIES; ++Entity)
	{
		AvailableEntities.push(Entity);
	}
}

LEntity LEntityManager::CreateEntity()
{
	assert(LivingEntityCount < MAX_ENTITIES);

	LEntity Id = AvailableEntities.front();
	AvailableEntities.pop();
	++LivingEntityCount;

	return Id;
}

void LEntityManager::DestroyEntity(LEntity Entity)
{
	assert(Entity < MAX_ENTITIES);

	Signatures[Entity].reset();

	AvailableEntities.push(Entity);
	--LivingEntityCount;
}

void LEntityManager::SetSignature(LEntity Entity, LSignature Signature)
{
	assert(Entity < MAX_ENTITIES);
	Signatures[Entity] = Signature;
}

LSignature LEntityManager::GetSignature(LEntity Entity)
{
	assert(Entity < MAX_ENTITIES);
	return Signatures[Entity];
}
