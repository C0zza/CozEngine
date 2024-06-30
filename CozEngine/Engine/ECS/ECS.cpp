#include "ECS.h"

void LECS::Update()
{
	for (LComponentSystemBase* ComponentSystem : TickableComponentSystems)
	{
		ComponentSystem->InternalRun();
	}
}
