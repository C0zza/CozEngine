#include "TestECSComponent.h"

#include <iostream>

void CTestECSComponentSystem::RunComponent(CTestECSComponent& Component)
{
	Component.SomeInt++;
	std::cout << Component.SomeInt << "\n";
}
