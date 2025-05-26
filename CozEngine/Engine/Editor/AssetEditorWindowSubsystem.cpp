#include "stdafx.h"

#include "AssetEditorWindowSubsystem.h"

#include <fstream>

#include "Editor/EditorWindows/AssetEditorWindow.h"
#include "Editor/LEditor.h"
#include "Globes.h"
#include "json.hpp"
#include "Misc/Logging.h"
#include "Reflection/ClassRegister.h"
#include "ResourceManagement/ResourceManager.h"

void LAssetEditorWindowSubsystem::RegisterAsset(const std::filesystem::path& AssetPath)
{
	if (!std::filesystem::exists(AssetPath))
	{
		Log(LLogLevel::ERROR, "LAssetEditorWindowSubsystem::RegisterAsset - AssetPath does not exist: " + AssetPath.string());
		return;
	}

	if (AssetPaths.contains(AssetPath))
	{
		return;
	}

	AssetPaths.emplace(AssetPath, FAssetInfo());

	if (AssetPaths.size() == 1)
	{
		LEditor* Editor = CSystem.GetSubsystems().GetSubsystem<LEditor>();
		if (!Editor)
		{
			Log(LLogLevel::ERROR, "LAssetEditorWindowSubsystem::RegisterAsset - Invalid Editor subsystem.");
			return;
		}

		Editor->RegisterWindow<LAssetEditorWindow>(AssetEditorWindowName.c_str());
	}

	std::ifstream File(AssetPath);  // Or "data.json"

	if (!File.is_open()) 
	{
		Log(LLogLevel::ERROR, "LAssetEditorWindowSubsystem::RegisterAsset - Failed to open file to determine type. " + AssetPath.string());
		return;
	}

	nlohmann::json Json;
	File >> Json;

	if (!Json.contains("Type"))
	{
		Log(LLogLevel::ERROR, "LAssetEditorWindowSubsystem::RegisterAsset - Type entry not found in asset: " + AssetPath.string());
		return;
	}

	LClass* Class = LClassRegister::Get(Json["Type"]);
	AssetPaths[AssetPath].Class = Class;

	if (!Class)
	{
		Log(LLogLevel::ERROR, "LAssetEditorWindowSubsystem::RegisterAsset - Failed to get LClass for " + Json["Type"]);
		return;
	}

	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	if (!ResourceManager)
	{
		Log(LLogLevel::ERROR, "LAssetEditorWindowSubsystem::RegisterAsset - Invalid Resource Manager");
		return;
	}

	ResourceManager->GetResource(AssetPath.string(), AssetPaths[AssetPath].Resource, Class);
}

void LAssetEditorWindowSubsystem::UnregisterAsset(const std::filesystem::path& AssetPath)
{
	if (!AssetPaths.contains(AssetPath))
	{
		Log(LLogLevel::INFO, "LAssetEditorWindowSubsystem::UnregisterAsset - AssetPath not found: " + AssetPath.string());
		return;
	}

	AssetPaths.erase(AssetPath);

	if (AssetPaths.empty())
	{
		LEditor* Editor = CSystem.GetSubsystems().GetSubsystem<LEditor>();
		if (!Editor)
		{
			Log(LLogLevel::ERROR, "LAssetEditorWindowSubsystem::RegisterAsset - Invalid Editor subsystem.");
			return;
		}

		Editor->UnregisterWindow(AssetEditorWindowName);
	}
}
