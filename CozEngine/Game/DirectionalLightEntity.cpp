#include "DirectionalLightEntity.h"

#include "ECS/ECSComponents/DirectionalLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"

CDirectionalLightEntity::CDirectionalLightEntity()
{
	CDirectionalLightComponent* DirectionalLight = AddComponent<CDirectionalLightComponent>();
	DirectionalLight->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	DirectionalLight->Diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	DirectionalLight->Specular = glm::vec3(1.0f, 1.0f, 1.0f);

	CTransformComponent* TransformComp = GetComponent<CTransformComponent>();
	TransformComp->SetRotation(glm::vec3(0.f, 45.f, 0.f));
}
