#include "Entity.h"

#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"
#include "Misc/TypeIdGenerator.h"

LEntity::LEntity()
{
	LTypeInstanceIdGenerator* TypeInstanceIdGenerator = CSystem.GetSubsystems().GetSubsystem<LTypeInstanceIdGenerator>();
	assert(TypeInstanceIdGenerator);
	
	ID = TypeInstanceIdGenerator->GetNewID<LEntity>();

	AddComponent<CTransformComponent>();
}

LEntity::~LEntity()
{
	GetECS()->RemoveEntity(ID);

	LTypeInstanceIdGenerator* TypeInstanceIdGenerator = CSystem.GetSubsystems().GetSubsystem<LTypeInstanceIdGenerator>();
	assert(TypeInstanceIdGenerator);

	TypeInstanceIdGenerator->UnregisterID<LEntity>(ID);
}

LECS* LEntity::GetECS() const
{
	LECS* ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	assert(ECS);
	return ECS;
}
