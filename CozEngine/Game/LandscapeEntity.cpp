#include "stdafx.h"

#include "LandscapeEntity.h"

#include "ECS/ECSComponents/LandscapeComponent.h"

CLandscapeEntity::CLandscapeEntity()
{
	AddComponent<CLandscapeComponent>("Game/Content/Textures/TestHeightMap.casset", "Game/Content/Textures/Ground.casset", "Game/Content/Textures/Wall.casset");
}
