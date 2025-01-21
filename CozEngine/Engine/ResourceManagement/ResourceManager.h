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
	virtual void Deinitialize() override;

	template<typename T>
	void GetResource(const std::string& Asset, LResourceHandle<T>& OutResourceHandle)
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

	void UnloadResource(const std::string Asset);

private:
	std::unordered_map<std::string, LResource*> Resources;
};

