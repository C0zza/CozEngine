#include "ECS.h"

void LECS::RunComponentSystems()
{
	for (auto& ComponentSystem : ComponentSystems)
	{
		ComponentSystem.second->Run();
	}
}
