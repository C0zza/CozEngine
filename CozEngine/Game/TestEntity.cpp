#include "TestEntity.h"

#include <memory>

// #include "Components/TestECSComponent.h"
#include "ECS/ECSComponents/ModelComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"

CTestEntity::CTestEntity(std::shared_ptr<LModel>& Model, std::shared_ptr<LMaterial>& Material)
{
	// AddComponent<CTestECSComponent>();
	AddComponent<CModelComponent>(Model, Material);
}
