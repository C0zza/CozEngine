#include "Entity.h"

#include "ECS/ECSComponents/TransformComponent.h"
#include "Misc/TypeIdGenerator.h"

LEntity::LEntity()
	: ID {LTypeIdGenerator<LEntityID>::GetNewID()}
{
	AddComponent<CTransformComponent>();
}

LEntity::~LEntity()
{
	LECS::Get()->RemoveEntity(ID);
	LTypeIdGenerator<LEntity>::UnregisterID(ID);
}
