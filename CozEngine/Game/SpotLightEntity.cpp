#include "SpotLightEntity.h"

#include "ECS/ECSComponents/ECSModelComponent.h"
#include "ECS/ECSComponents/ECSPointLightComponent.h"
#include "ECS/ECSComponents/ECSSpotLightComponent.h"
#include "ECS/ECSComponents/ECSTransformComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"

CSpotLightEntity::CSpotLightEntity(std::shared_ptr<LModel>& Model, std::shared_ptr<LMaterial>& Material)
{
	CECSPointLightComponent* SpotLight = AddComponent<CECSPointLightComponent>();
	SpotLight->SetAmbient(glm::vec3(0.f, 0.2f, 0.f));
	SpotLight->SetDiffuse(glm::vec3(0.f, 0.5f, 0.f));
	SpotLight->SetSpecular(glm::vec3(0.f, 0.7f, 0.f));
	//SpotLight->SetCutOff(glm::cos(glm::radians(12.5f)));
	//SpotLight->SetOuterCutOff(glm::cos(glm::radians(17.5f)));
	SpotLight->SetConstant(1.0f);
	SpotLight->SetLinear(0.09f);
	SpotLight->SetQuadratic(0.032f);

	AddComponent<CECSModelComponent>(Model, Material);

	CECSTransformComponent* Transform = GetComponent<CECSTransformComponent>();
	Transform->Move(glm::vec3(0.f, 0.f, 5.f));
}
