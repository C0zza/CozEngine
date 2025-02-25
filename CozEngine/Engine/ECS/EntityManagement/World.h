#pragma once

#include "ECS/EntityManagement/EntityContainer.h"
#include "ECS/EntityManagement/EntityContainerAsset.h"
#include "ResourceManagement/ResourceHandle.h"

class LWorld
{
public:
	LWorld() = default;
	LWorld(const FAssetPath& InEntitiesData);

	void SaveWorld(const FAssetPath& AssetPath);
	bool HasAsset() const;

	LEntityContainer EntityContainer;
private:
	LResourceHandle<LEntityContainerAsset> EntitiesData;
};
