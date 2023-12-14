#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

struct CTestECSComponent : LECSComponent
{
	int SomeInt = 0;
};

class CTestECSComponentSystem : public LComponentSystem<CTestECSComponent>
{
	virtual void RunComponent(CTestECSComponent& Component) override;
};

