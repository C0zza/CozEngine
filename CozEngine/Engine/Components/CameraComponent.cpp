#include "CameraComponent.h"

#include <glm/gtx/rotate_vector.hpp>

#include "Object.h"

CCameraComponent* CCameraComponent::ActiveCamera = nullptr;

void CCameraComponent::Init()
{
    GetViewMatrix();
}

const glm::mat4& CCameraComponent::GetViewMatrix()
{
    assert(Parent);
    if (Parent->Transform.GetIsDirty())
    {
        glm::vec3 ForwardVector = Parent->Transform.GetForward();
        CachedViewMatrix = glm::lookAt(Parent->Transform.GetPosition(), glm::vec3(ForwardVector.x, ForwardVector.y, ForwardVector.z), glm::vec3(0.f, 1.f, 0.f));
    }

    return CachedViewMatrix;
}
