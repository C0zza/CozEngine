#pragma once

#include <filesystem>
#include <set>

#include "Subsystem.h"

class LAssetEditorWindowSubsystem : public LSubsystem
{
public:
	void RegisterAsset(const std::filesystem::path& AssetPath);
	void UnregisterAsset(const std::filesystem::path& AssetPath);

	const std::set<std::filesystem::path>& GetRegisteredAssets() const { return AssetPaths; }

private:
	std::set<std::filesystem::path> AssetPaths;

	const std::string AssetEditorWindowName = "Asset Editor";
};

