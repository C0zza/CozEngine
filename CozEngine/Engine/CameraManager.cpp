#include "stdafx.h"

#include "CameraManager.h"

#include "ECS/ECS2/ArchetypeManager.h"
#include "ECS/ECSComponents/CameraComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"

namespace
{
    glm::vec3 ZeroVec3(0.f, 0.f, 0.f);
    glm::mat4 IdentityMat4(1.f);
}

void LCameraManager::Initialize()
{
    ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
    assert(ArchetypeManager);
}

const glm::mat4& LCameraManager::GetViewMatrix()
{
    CCameraComponent* CameraComponent = ArchetypeManager->GetEntityComponent<CCameraComponent>(ActiveCameraEntityID);
    CTransformComponent* TransformComponent = ArchetypeManager->GetEntityComponent<CTransformComponent>(ActiveCameraEntityID);

    if (!CameraComponent || !TransformComponent)
    {
        return IdentityMat4;
    }

    TransformComponent->UpdateTransformationMatrix();
    CameraComponent->CachedViewMatrix = glm::lookAt(TransformComponent->GetPosition(), TransformComponent->GetPosition() + TransformComponent->GetForward(), glm::vec3(0.f, 1.f, 0.f));

    return CameraComponent->CachedViewMatrix;
}

const glm::vec3& LCameraManager::GetViewPos()
{
    CTransformComponent* TransformComponent = ArchetypeManager->GetEntityComponent<CTransformComponent>(ActiveCameraEntityID);
    if (!TransformComponent)
    {
        return ZeroVec3;
    }

    return TransformComponent->GetPosition();
}