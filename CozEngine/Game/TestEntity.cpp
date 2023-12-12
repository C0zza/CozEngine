#include "TestEntity.h"

#include <memory>

// #include "Components/TestECSComponent.h"
#include "ECS/ECSComponents/ECSModelComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"

CTestEntity::CTestEntity(LECS* i_ECS, std::shared_ptr<LModel>& Model, std::shared_ptr<LMaterial>& Material)
	: LEntity(i_ECS)
{
	// AddComponent<CTestECSComponent>();
	AddComponent<CECSModelComponent>(Model, Material);
}
