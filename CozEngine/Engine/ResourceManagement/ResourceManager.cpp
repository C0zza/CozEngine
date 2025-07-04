#include "ResourceManager.h"

#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"

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

bool LResourceManager::IsAssetPathType(const FAssetPath& Asset, LClass* Class) const
{
	std::ifstream File(Asset);

	if (!File.is_open())
	{
		return false;
	}

	nlohmann::json J;
	File >> J;

	if (!J.contains("Type"))
	{
		return false;
	}

	LClass* ResourceClass = LClassRegister::Get(J["Type"]);

	if (!ResourceClass)
	{
		return false;
	}

	return ResourceClass == Class;
}

void LResourceManager::OnAssetPathUpdated(const std::filesystem::path& OldPath, const std::filesystem::path& NewPath)
{
	if (!Resources.contains(OldPath))
	{
		return;
	}

	LResource* Resource = Resources.at(OldPath);

	if (Resource)
	{
		Resource->SetAssetPath(NewPath);
	}

	Resources.try_emplace(NewPath, Resources.at(OldPath));

	Resources.erase(OldPath);
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
