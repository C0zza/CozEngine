#include "TestEntity.h"

#include "Components/TestECSComponent.h"

CTestEntity::CTestEntity(LECS* i_ECS)
	: LEntity(i_ECS)
{
	AddComponent<CTestECSComponent>();
}
