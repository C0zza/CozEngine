#include "ECS.h"

void LECS::RunComponentSystems()
{
	for (LComponentSystemBase* ComponentSystem : TickableComponentSystems)
	{
		ComponentSystem->Run();
	}
}
