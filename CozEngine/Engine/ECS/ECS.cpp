#include "ECS.h"

#include "Globes.h"

LECS::LECS()
{
	TypeIdGenerator = CSystem.GetSubsystems().GetSubsystem<LTypeIdGenerator>();
	assert(TypeIdGenerator);
}

void LECS::Update()
{
	for (LComponentSystemBase* ComponentSystem : TickableComponentSystems)
	{
		ComponentSystem->InternalRun();
	}
}
