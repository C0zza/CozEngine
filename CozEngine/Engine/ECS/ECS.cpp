#include "ECS.h"

LECS::LECS()
{
	LComponentSystemBase::SetECS(this);
}

void LECS::RunComponentSystems()
{
	for (LComponentSystemBase* ComponentSystem : TickableComponentSystems)
	{
		ComponentSystem->Run();
	}
}
