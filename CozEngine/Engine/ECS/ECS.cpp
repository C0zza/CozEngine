#include "ECS.h"

void LECS::Tick()
{
	for (LComponentSystemBase* ComponentSystem : TickableComponentSystems)
	{
		ComponentSystem->InternalRun();
	}
}
