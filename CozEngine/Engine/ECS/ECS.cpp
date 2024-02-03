#include "ECS.h"

#include <cassert>

void LECS::RunComponentSystems()
{
	for (LComponentSystemBase* ComponentSystem : TickableComponentSystems)
	{
		ComponentSystem->Run();
	}
}
