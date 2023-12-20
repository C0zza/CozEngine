#include "DirectionalLightEntity.h"

#include "ECS/ECSComponents/DirectionalLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"

CDirectionalLightEntity::CDirectionalLightEntity()
{
	CDirectionalLightComponent* DirectionalLight = AddComponent<CDirectionalLightComponent>();
	DirectionalLight->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	DirectionalLight->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	DirectionalLight->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

	CTransformComponent* TransformComp = GetComponent<CTransformComponent>();
	TransformComp->SetRotation(glm::vec3(0.f, 45.f, 0.f));
}
