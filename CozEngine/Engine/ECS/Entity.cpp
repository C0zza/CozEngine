#include "Entity.h"

#include "ECS/ECSComponents/ECSTransformComponent.h"
#include "Misc/TypeIdGenerator.h"

LEntity::LEntity(LECS* i_ECS)
	: ID {LTypeIdGenerator<LEntityID>::GetNewID()}, ECS{i_ECS}
{
	AddComponent<CECSTransformComponent>();
}

LEntity::~LEntity()
{
	ECS->RemoveEntity(ID);
	LTypeIdGenerator<LEntity>::UnregisterID(ID);
}
