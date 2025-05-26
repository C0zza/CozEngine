#pragma once

#include <filesystem>
#include <set>

#include "ResourceManagement/ResourceHandle.h"
#include "Subsystem.h"

class LClass;

struct FAssetInfo
{
	LClass* Class = nullptr;
	LResourceHandle<LResource> Resource;
};

class LAssetEditorWindowSubsystem : public LSubsystem
{
public:
	void RegisterAsset(const std::filesystem::path& AssetPath);
	void UnregisterAsset(const std::filesystem::path& AssetPath);

	const std::unordered_map<std::filesystem::path, FAssetInfo>& GetRegisteredAssets() const { return AssetPaths; }

private:
	std::unordered_map<std::filesystem::path, FAssetInfo> AssetPaths;

	const std::string AssetEditorWindowName = "Asset Editor";
};

