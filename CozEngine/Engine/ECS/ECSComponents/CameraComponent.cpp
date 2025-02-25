#include "CameraComponent.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/vec3.hpp>

#include "ECS/ECS.h"
#include "Misc/GlmSerialization.h"
#include "Globes.h"
#include "Misc/Logging.h"
#include "TransformComponent.h"

const glm::mat4& CCameraComponentSystem::GetViewMatrix()
{
    CCameraComponent* CameraComponent = ECS->GetComponent<CCameraComponent>(ActiveCameraEntityID);
    CTransformComponent* CameraTransform = ECS->GetComponent<CTransformComponent>(ActiveCameraEntityID);
    assert(CameraComponent && CameraTransform);

    CameraTransform->UpdateTransformationMatrix();
    CameraComponent->CachedViewMatrix = glm::lookAt(CameraTransform->GetPosition(), CameraTransform->GetPosition() + CameraTransform->GetForward(), glm::vec3(0.f, 1.f, 0.f));

    return CameraComponent->CachedViewMatrix;
}

const glm::vec3& CCameraComponentSystem::GetViewPos()
{
    assert(ECS);
    CTransformComponent* CameraTransform = ECS->GetComponent<CTransformComponent>(ActiveCameraEntityID);
    assert(CameraTransform);
    return CameraTransform->GetPosition();
}

void CCameraComponentSystem::GetSerializedComponent(const CCameraComponent& Component, nlohmann::json& J) const
{
    J["CachedViewMatrix"] = Component.CachedViewMatrix;
}

void CCameraComponentSystem::DeserializeComponent(CCameraComponent& Component, const nlohmann::json& J)
{
    Component.CachedViewMatrix = J["CachedViewMatrix"];
}
