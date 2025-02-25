#include "stdafx.h"
#include "World.h"

#include "Globes.h"
#include "Misc/Logging.h"
#include "ResourceManagement/ResourceManager.h"

LWorld::LWorld(const FAssetPath& InEntitiesDataPath)
{
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	if (!ResourceManager)
	{
		Log(LLogLevel::ERROR, "LWorld::LWorld - Invalid ResourceManager. Unable to load world: " + InEntitiesDataPath);
		return;
	}

	ResourceManager->GetResource<LEntityContainerAsset>(InEntitiesDataPath, EntitiesData);

	bool bSuccessfullyLoaded = false;
	EntityContainer = LEntityContainer(EntitiesData->GetLevelAssetData(), bSuccessfullyLoaded);

	if (!bSuccessfullyLoaded)
	{
		Log(LLogLevel::ERROR, "LWorld::LWorld - Failed to load InEntitiesDataPath: " + InEntitiesDataPath);
		return;
	}
}

#if defined(COZ_EDITOR)
void LWorld::SaveWorld(const FAssetPath& AssetPath)
{
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	if (!ResourceManager)
	{
		Log(LLogLevel::ERROR, "LWorld::SaveWorld - ResourceManager invalid. Unable to save world: " + AssetPath);
		return;
	}

	if (!HasAsset())
	{
		ResourceManager->SaveResource<LEntityContainerAsset>(AssetPath, EntitiesData);
	} 
	else if(EntitiesData.Get()->GetAssetPath() != AssetPath)
	{
		Log(LLogLevel::WARNING, "LWorld::SaveWorld - Cannot save world to " + AssetPath + ". It is already saved to " + EntitiesData.Get()->GetAssetPath() + ".");
		return;
	}

	EntitiesData->UpdateLevelAssetData(EntityContainer);
	ResourceManager->SaveResource<LEntityContainerAsset>(AssetPath, EntitiesData);
}

bool LWorld::HasAsset() const
{
	return EntitiesData.Get();
}
#endif
