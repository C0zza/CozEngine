#pragma once

#include "ECS/ECSComponents/ECSComponent.h"
#include "Transform.h"

// Temporarily inherit LTransform to avoid rewriting it all here
struct CTransformComponent : public LTransform, LECSComponent
{
};

