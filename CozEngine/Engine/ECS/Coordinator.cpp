#include "Coordinator.h"

void LCoordinator::Init()
{
	ComponentManager = std::make_unique<LComponentManager>();
	EntityManager = std::make_unique<LEntityManager>();
	SystemManager = std::make_unique<LSystemManager>();
}

LEntity LCoordinator::CreateEntity()
{
	return EntityManager->CreateEntity();
}

void LCoordinator::DestroyEntity(LEntity Entity)
{
	EntityManager->DestroyEntity(Entity);
	ComponentManager->EntityDestroyed(Entity);
	SystemManager->EntityDestroyed(Entity);
}
