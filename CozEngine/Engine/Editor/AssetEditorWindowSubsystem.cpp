#include "stdafx.h"

#include "AssetEditorWindowSubsystem.h"

#include "Editor/EditorWindows/AssetEditorWindow.h"
#include "Editor/LEditor.h"
#include "Globes.h"
#include "Misc/Logging.h"

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

	AssetPaths.emplace(AssetPath);

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
