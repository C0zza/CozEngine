#include "DirectionalLightEntity.h"

#include "ECS/ECSComponents/ECSDirectionalLightComponent.h"
#include "ECS/ECSComponents/ECSTransformComponent.h"

CDirectionalLightEntity::CDirectionalLightEntity()
{
	CECSDirectionalLightComponent* DirectionalLight = AddComponent<CECSDirectionalLightComponent>();
	DirectionalLight->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	DirectionalLight->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	DirectionalLight->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

	CECSTransformComponent* TransformComp = GetComponent<CECSTransformComponent>();
	TransformComp->SetRotation(glm::vec3(0.f, 45.f, 0.f));
}
