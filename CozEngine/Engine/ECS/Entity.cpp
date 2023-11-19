#include "Entity.h"

#include "Misc/TypeIdGenerator.h"

LEntity::LEntity(LECS* i_ECS)
	: ID {LTypeIdGenerator<LEntityID>::GetNewID()}, ECS{i_ECS}
{
}

LEntity::~LEntity()
{
	ECS->RemoveEntity(ID);
	LTypeIdGenerator<LEntity>::UnregisterID(ID);
}
