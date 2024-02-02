#include "ResourceManager.h"

std::unordered_map<std::string, LResource*> LResourceManager::Resources{};

void LResourceManager::Shutdown()
{
	for (auto& Resource : Resources)
	{
		Resource.second->Unload();
		delete Resource.second;
		Resource.second = nullptr;
	}
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
