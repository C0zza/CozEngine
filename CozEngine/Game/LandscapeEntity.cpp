#include "stdafx.h"

#include "LandscapeEntity.h"

#include "ECS/ECSComponents/LandscapeComponent.h"

CLandscapeEntity::CLandscapeEntity()
{
	AddComponent<CLandscapeComponent>("Game/Content/Materials/MAT_LandscapeMaterial.casset");
}
