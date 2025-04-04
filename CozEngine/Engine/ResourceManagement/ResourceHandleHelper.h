#pragma once

#include "ResourceManager.h"

#include "Globes.h"
#include "json.hpp"

template<typename T>
inline void to_json(nlohmann::json& J, const LResourceHandle<T>& ResourceHandle)
{
	J = nlohmann::json{ {"AssetPath", ""} };

	if (LResource* Resource = dynamic_cast<LResource*>(ResourceHandle.Get()))
	{
		J["AssetPath"] = Resource->GetAssetPath();
	}
}

template<typename T>
inline void from_json(const nlohmann::json& J, LResourceHandle<T>& ResourceHandle)
{
	std::string AssetPath;
	J.at("AssetPath").get_to(AssetPath);

	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();

	ResourceManager->GetResource<T>(AssetPath, ResourceHandle);
}

