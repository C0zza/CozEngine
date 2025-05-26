#include "Resource.h"

#include <cassert>

#include "Globes.h"
#include "ResourceManager.h"

void LResource::IncrementRefs()
{
	++RefCount;
}

void LResource::DecrementRefs()
{
	assert(RefCount > 0);
	--RefCount;

	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	if (!RefCount)
	{
		ResourceManager->UnloadResource(AssetPath.string());
	}
}

void LResource::SetAssetPath(const FAssetPath& i_AssetPath)
{
	// Make sure we're only setting the asset path during init
	assert(!RefCount);
	AssetPath = i_AssetPath;
}
