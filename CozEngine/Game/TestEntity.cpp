#include "TestEntity.h"

#include <memory>

#include "ECS/ECSComponents/ModelComponent.h"

CTestEntity::CTestEntity()
{
	AddComponent<CModelComponent>("Game/Content/Models/MOD_Backpack.casset", "Game/Content/Materials/MAT_DefaultMaterial.casset");
}
