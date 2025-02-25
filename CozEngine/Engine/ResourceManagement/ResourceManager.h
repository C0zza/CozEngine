#pragma once

#include <unordered_map>
#include <string>

#include "Resource.h"
#include "ResourceHandle.h"
#include "Serializable.h"
#include "Subsystem.h"

class LResourceManager : public LSubsystem
{
public:
	~LResourceManager();

	template<typename T>
	void GetResource(const FAssetPath& Asset, LResourceHandle<T>& OutResourceHandle)
	{
		if (Asset.empty())
		{
			Log(LLogLevel::INFO, "LResourceManager::GetResource - Empty Asset provided. Returning empty LResourceHandle.");
			return;
		}

		if (!Resources.contains(Asset))
		{
			T* Resource = new T();
			Resources.insert({ Asset, Resource });

			LSavable::LoadAssetFromDisk(Asset, *Resource);
			Resource->SetAssetPath(Asset);
			Resource->Load();
		}

		OutResourceHandle.Init(Resources[Asset]);
	}

	template<typename T>
	void SaveResource(const FAssetPath& AssetPath, LResourceHandle<T>& OutResourceHandle)
	{
		if (AssetPath.empty())
		{
			Log(LLogLevel::WARNING, "LResourceManager::CreateResourceAsset - Empty AssetPath provided. " + AssetPath + " Returning empty LResourceHandle.");
			return;
		}

		if (!Resources.contains(AssetPath))
		{
			std::ifstream File(AssetPath);
			if (File.good())
			{
				Log(LLogLevel::INFO, "LResourceManager::CreateResourceAsset - AssetPath: " + AssetPath + " already exists.");
				GetResource<T>(AssetPath, OutResourceHandle);
			}
			else
			{
				T* Resource = new T();
				Resources.insert({AssetPath, Resource});
				Resource->SetAssetPath(AssetPath);
				OutResourceHandle.Init(Resource);
			}
		}
		else
		{
			Log(LLogLevel::INFO, "LResourceManager::CreateResourceAsset - AssetPath: " + AssetPath + " already loaded.");
			OutResourceHandle.Init(Resources[AssetPath]);
		}

		LSavable::SaveAssetToDisk(AssetPath, *OutResourceHandle.Get());
	}

	void UnloadResource(const std::string Asset);

private:
	std::unordered_map<FAssetPath, LResource*> Resources;
};

