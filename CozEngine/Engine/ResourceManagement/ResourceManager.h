#pragma once

#include <unordered_map>
#include <string>

#include "Resource.h"
#include "ResourceHandle.h"
#include "Serializable.h"

class LResourceManager
{
public:
	static void Shutdown();

	template<typename T>
	static LResourceHandle<T> GetResource(const std::string& Asset)
	{
		if (Asset.empty())
		{
			Log(LLogLevel::INFO, "LResourceManager::GetResource - Empty Asset provided. Returning empty LResourceHandle.");
			return LResourceHandle<T>();
		}

		if (!Resources.contains(Asset))
		{
			T* Resource = new T();
			Resources.insert({ Asset, Resource });

			LSavable::LoadAssetFromDisk(Asset, *Resource);
			Resource->SetAssetPath(Asset);
			Resource->Load();
		}

		return LResourceHandle<T>(Resources[Asset]);
	}

	static void UnloadResource(const std::string Asset);

private:
	static std::unordered_map<std::string, LResource*> Resources;
};

