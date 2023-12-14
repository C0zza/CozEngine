#include "Entity.h"

#include "ECS/ECSComponents/ECSTransformComponent.h"
#include "Misc/TypeIdGenerator.h"

LEntity::LEntity()
	: ID {LTypeIdGenerator<LEntityID>::GetNewID()}
{
	AddComponent<CECSTransformComponent>();
}

LEntity::~LEntity()
{
	LECS::Get()->RemoveEntity(ID);
	LTypeIdGenerator<LEntity>::UnregisterID(ID);
}
