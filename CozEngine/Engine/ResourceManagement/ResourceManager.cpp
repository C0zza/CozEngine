#include "ResourceManager.h"

#include "Reflection/Class.h"

LResourceManager::~LResourceManager()
{
	if (!Resources.empty())
	{
		std::string ErrorString = "LResourceManager::~LResourceManager - Resources have not been released:";
		for (const std::pair<FAssetPath, LResource*>& Pair : Resources)
		{
			ErrorString += "\n - " + Pair.first.string();
		}
		Log(LLogLevel::ERROR, ErrorString);
	}
}

void LResourceManager::GetResource(const FAssetPath& Asset, LResourceHandle<LResource>& OutResourceHandle, LClass* Class)
{
	if (!Class)
	{
		Log(LLogLevel::ERROR, "LResourceManager::GetResource - Invalid Class.");
		return;
	}

	assert(Class->IsChildOf<LResource>());

	if (Asset.empty() || Asset == "null")
	{
		Log(LLogLevel::INFO, "LResourceManager::GetResource - Empty Asset provided. Returning empty LResourceHandle.");
		return;
	}

	if (!Resources.contains(Asset))
	{
		LResource* Resource = Class->CreateObject<LResource>();
		Resources.insert({ Asset, Resource });

		LSavable::LoadAssetFromDisk(Asset.string(), Class, Resource);
		Resource->SetAssetPath(Asset);
		Resource->Load();
	}

	OutResourceHandle.Init(Resources[Asset]);
}

void LResourceManager::SaveResource(const FAssetPath& Asset, LResourceHandle<LResource>& OutResourceHandle, LClass* Class)
{
	if (!Class)
	{
		Log(LLogLevel::ERROR, "LResourceManager::GetResource - Invalid Class.");
		return;
	}

	assert(Class->IsChildOf<LResource>());

	if (Asset.empty() || Asset == "null")
	{
		Log(LLogLevel::INFO, "LResourceManager::GetResource - Empty Asset provided. Returning empty LResourceHandle.");
		return;
	}

	if (!Resources.contains(Asset))
	{
		std::ifstream File(Asset);
		if (File.good())
		{
			Log(LLogLevel::INFO, "LResourceManager::CreateResourceAsset - AssetPath: " + Asset.string() + " already exists.");
		}
		else
		{
			if (!OutResourceHandle.Get())
			{
				OutResourceHandle.Init(Class->CreateObject<LResource>());
			}
			
			Resources.insert({ Asset, OutResourceHandle.Get()});
			OutResourceHandle.Get()->SetAssetPath(Asset);
		}
	}
	else
	{
		Log(LLogLevel::INFO, "LResourceManager::CreateResourceAsset - AssetPath: " + Asset.string() + " already loaded.");
		OutResourceHandle.Init(Resources[Asset]);
	}

	LSavable::SaveAssetToDisk(Asset.string(), Class, OutResourceHandle.Get());
}

// Asset string not passed as reference because we need to null Resources[Asset] and Asset would otherwise be deleted
void LResourceManager::UnloadResource(const std::string Asset)
{
	if (Resources.contains(Asset))
	{
		Resources[Asset]->Unload();

		delete Resources[Asset];
		Resources[Asset] = nullptr;

		Resources.erase(Asset);
	}
	else
	{
		Log(LLogLevel::INFO, "LResourceManager::UnloadResource - Attempted to unload resource, " + Asset + " which isn't currently loaded.");
	}
}
