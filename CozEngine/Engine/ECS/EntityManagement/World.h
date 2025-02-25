#pragma once

#include "ECS/EntityManagement/EntityContainer.h"
#include "ECS/EntityManagement/EntityContainerAsset.h"
#include "ResourceManagement/ResourceHandle.h"

class LWorld
{
public:
	LWorld() = default;
	LWorld(const FAssetPath& InEntitiesData);

#if defined(COZ_EDITOR)
	void SaveWorld(const FAssetPath& AssetPath);
	bool HasAsset() const;
#endif

	LEntityContainer EntityContainer;
private:
	LResourceHandle<LEntityContainerAsset> EntitiesData;
};
