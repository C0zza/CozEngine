#include "ResourceManager.h"

void LResourceManager::Deinitialize()
{
	if (!Resources.empty())
	{
		std::string ErrorString = "LResourceManager::Deinitialize - Resources have not been released:";
		for (const std::pair<std::string, LResource*>& Pair : Resources)
		{
			ErrorString += "\n - " + Pair.first;
		}
		Log(LLogLevel::ERROR, ErrorString);
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
