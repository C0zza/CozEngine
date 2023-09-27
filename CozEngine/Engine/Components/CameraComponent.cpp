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
        CachedViewMatrix = glm::lookAt(Parent->Transform.GetPosition(), Parent->Transform.GetPosition() + Parent->Transform.GetForward(), Parent->Transform.GetUp());
    }

    return CachedViewMatrix;
}
