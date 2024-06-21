#include "SpotLightEntity.h"

#include "ECS/ECSComponents/ModelComponent.h"
#include "ECS/ECSComponents/PointLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"

CSpotLightEntity::CSpotLightEntity()
{
	//CPointLightComponent* SpotLight = AddComponent<CPointLightComponent>();
	//SpotLight->SetAmbient(glm::vec3(0.f, 0.2f, 0.f));
	//SpotLight->SetDiffuse(glm::vec3(0.f, 0.5f, 0.f));
	//SpotLight->SetSpecular(glm::vec3(0.f, 0.7f, 0.f));
	////SpotLight->SetCutOff(glm::cos(glm::radians(12.5f)));
	////SpotLight->SetOuterCutOff(glm::cos(glm::radians(17.5f)));
	//SpotLight->SetConstant(1.0f);
	//SpotLight->SetLinear(0.09f);
	//SpotLight->SetQuadratic(0.032f);

	AddComponent<CModelComponent>("Game/Content/Models/MOD_Cube.casset", "Game/Content/Materials/MAT_Cube.casset");

	CTransformComponent* Transform = GetComponent<CTransformComponent>();
	Transform->Move(glm::vec3(0.f, 0.f, 5.f));
}
