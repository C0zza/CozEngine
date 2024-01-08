#include "Resource.h"

#include <cassert>

#include "ResourceManager.h"

void LResource::IncrementRefs()
{
	++RefCount;
}

void LResource::DecrementRefs()
{
	assert(RefCount > 0);
	--RefCount;

	if (!RefCount)
	{
		LResourceManager::UnloadResource(AssetPath);
	}
}

void LResource::SetAssetPath(const std::string& i_AssetPath)
{
	// Make sure we're only setting the asset path during init
	assert(!RefCount);
	AssetPath = i_AssetPath;
}
