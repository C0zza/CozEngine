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
	GetECS()->RemoveEntity(ID);
	LTypeIdGenerator<LEntity>::UnregisterID(ID);
}

LECS* LEntity::GetECS() const
{
	LECS* ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	assert(ECS);
	return ECS;
}
