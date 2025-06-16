#pragma once

#include <filesystem>

#include "Globes.h"
#include "ResourceManagement/ResourceHandle.h"
#include "ResourceManagement/ResourceManager.h"

template<typename T>
class LSoftResourceHandle
{
public:
	void Load(LResourceHandle<T>& OutResourceHandle);

	std::filesystem::path ResourcePath;
};

template<typename T>
inline void LSoftResourceHandle<T>::Load(LResourceHandle<T>& OutResourceHandle)
{
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	assert(ResourceManager);

	LResource* Resource = ResourceManager->GetResource(ResourcePath.string(), OutResourceHandle, T::StaticClass());
}
