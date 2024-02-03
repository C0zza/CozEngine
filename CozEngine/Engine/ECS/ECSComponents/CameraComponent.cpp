#include "CameraComponent.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/vec3.hpp>

#include "ECS/ECS.h"
#include "Globes.h"
#include "TransformComponent.h"

LEntityID CCameraComponent::ActiveCameraEntityID = 0;

const glm::mat4& CCameraComponent::GetViewMatrix()
{
    assert(ECS);
    CTransformComponent* EntityTransform = ECS->GetComponent<CTransformComponent>(EntityID);
    assert(EntityTransform);

    if(EntityTransform)
    {
        EntityTransform->UpdateTransformationMatrix();
        CachedViewMatrix = glm::lookAt(EntityTransform->GetPosition(), EntityTransform->GetPosition() + EntityTransform->GetForward(), glm::vec3(0.f, 1.f, 0.f));
    }

    return CachedViewMatrix;
}

const glm::vec3& CCameraComponent::GetViewPos()
{
    assert(ECS);
    CTransformComponent* EntityTransform = ECS->GetComponent<CTransformComponent>(EntityID);
    assert(EntityTransform);
    return EntityTransform->GetPosition();
}
