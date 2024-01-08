#pragma once

#include <unordered_map>
#include <string>

#include "Resource.h"

class LResourceManager
{
public:
	static void Shutdown();

	template<typename T>
	static LResourceHandle<T> GetResource(const std::string& Asset)
	{
		if (!Resources.contains(Asset))
		{
			Resources.insert({ Asset, new T() });
			Resources[Asset]->SetAssetPath(Asset);
			Resources[Asset]->LoadAssetFromDisk(Asset, *Resources[Asset]);
			Resources[Asset]->Load();
		}

		LResourceHandle<T> Resource(Resources[Asset]);
		return Resource;
	}

	static void UnloadResource(const std::string& Asset);

private:
	static std::unordered_map<std::string, LResource*> Resources;
};

