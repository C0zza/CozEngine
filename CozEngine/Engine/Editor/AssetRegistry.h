#pragma once

#include <filesystem>
#include <unordered_set>

#include "Subsystem.h"

class LClass;

class LAssetRegistry : public LSubsystem
{
public:
	virtual void Initialize() override;

	std::unordered_set<std::string_view> GetAssetsByClass(LClass* Class) const;

private:
	struct FNode
	{
		bool IsValid() const;
		bool IsDirectory() const;

		std::string Path;
		std::vector<FNode> Contents;
	};

	FNode RootNode;

private:
	void RegisterRootPath(const std::filesystem::path& Path);
	void RegisterPath(FNode* Parent, const std::filesystem::path& Path);
	void RegisterAsset(FNode* Parent, const std::filesystem::path& Path);

	std::map<LClass*, std::unordered_set<std::string_view>> ClassToAssetPathsMap;
};

