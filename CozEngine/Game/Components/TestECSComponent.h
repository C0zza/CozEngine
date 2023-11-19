#pragma once

#include "ECS/ComponentSystem.h"

struct CTestECSComponent
{
	int SomeInt = 0;
};

class CTestECSComponentSystem : public LComponentSystem<CTestECSComponent>
{
	virtual void RunComponent(CTestECSComponent& Component) override;
};

