#include "ECS.h"

#include <cassert>

LECS* LECS::ECS = nullptr;

LECS::LECS()
{	
	assert(!ECS);
	ECS = this;
}

void LECS::RunComponentSystems()
{
	for (LComponentSystemBase* ComponentSystem : TickableComponentSystems)
	{
		ComponentSystem->Run();
	}
}
