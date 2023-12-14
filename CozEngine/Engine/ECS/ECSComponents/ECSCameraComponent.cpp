#include "ECSCameraComponent.h"

#include <glm/gtx/rotate_vector.hpp>

#include "ECS/ECS.h"
#include "ECSTransformComponent.h"

LEntityID CECSCameraComponent::ActiveCameraEntityID = 0;

const glm::mat4& CECSCameraComponent::GetViewMatrix()
{
    CECSTransformComponent* EntityTransform = LECS::Get()->GetComponent<CECSTransformComponent>(EntityID);
    assert(EntityTransform);

    if(EntityTransform)
    {
        EntityTransform->UpdateTransformationMatrix();
        CachedViewMatrix = glm::lookAt(EntityTransform->GetPosition(), EntityTransform->GetPosition() + EntityTransform->GetForward(), glm::vec3(0.f, 1.f, 0.f));
    }

    return CachedViewMatrix;
}

const glm::vec3& CECSCameraComponent::GetViewPos()
{
    CECSTransformComponent* EntityTransform = LECS::Get()->GetComponent<CECSTransformComponent>(EntityID);
    assert(EntityTransform);
    return EntityTransform->GetPosition();
}
