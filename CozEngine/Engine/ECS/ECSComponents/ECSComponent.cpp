#include "ECSComponent.h"

#include "ECS/ECS.h"
#include "Globes.h"

void LECSComponent::InternalInit()
{
    ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
    if (!ECS)
    {
        Log(LLogLevel::WARNING, "LECSComponent::InternalInit - Failed to get LECS subsystem for component");
        return;
    }

    Init();
}
