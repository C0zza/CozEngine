#include "SpotLightEntity.h"

#include "Components/MovementComponent.h"
#include "ECS/ECSComponents/ModelComponent.h"
#include "ECS/ECSComponents/PointLightComponent.h"
#include "ECS/ECSComponents/SpotLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"

CSpotLightEntity::CSpotLightEntity()
{
	CSpotLightComponent* SpotLight = AddComponent<CSpotLightComponent>();
	SpotLight->Ambient = glm::vec3(0.f, 0.2f, 0.f);
	SpotLight->Diffuse = glm::vec3(0.f, 0.5f, 0.f);
	SpotLight->Specular = glm::vec3(0.f, 0.7f, 0.f);
	SpotLight->CutOff = glm::cos(glm::radians(12.5f));
	SpotLight->OuterCutOff = glm::cos(glm::radians(17.5f));
	SpotLight->Constant = 1.0f;
	SpotLight->Linear = 0.09f;
	SpotLight->Quadratic = 0.032f;

	AddComponent<CModelComponent>("Game/Content/Models/MOD_Cube.casset", "Game/Content/Materials/MAT_Cube.casset");

	CTransformComponent* Transform = GetComponent<CTransformComponent>();
	Transform->Move(glm::vec3(0.f, 0.f, 5.f));

	CMovementComponent* MovementComponent = AddComponent<CMovementComponent>();
	MovementComponent->Rotation.y = 0.4f;
}
