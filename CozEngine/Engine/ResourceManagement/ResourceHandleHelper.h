#pragma once

#include "ResourceManager.h"

#include "Globes.h"
#include "json.hpp"

class LResourceHandleHelper
{
public:
	template<typename T>
	static void ToJsonHelper(nlohmann::json& J, const LResourceHandle<T>& ResourceHandle)
	{
		J = nlohmann::json{ {"AssetPath", ""} };

		if (LResource* Resource = dynamic_cast<LResource*>(ResourceHandle.Get()))
		{
			J["AssetPath"] = Resource->GetAssetPath();
		}
	}

	template<typename T>
	static void FromJsonHelper(const nlohmann::json& J, LResourceHandle<T>& ResourceHandle)
	{
		std::string AssetPath;
		J.at("AssetPath").get_to(AssetPath);

		LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();

		ResourceManager->GetResource<T>(AssetPath, ResourceHandle);
	}
};

