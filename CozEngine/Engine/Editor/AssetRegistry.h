#pragma once

#include <filesystem>
#include <unordered_set>
#include <set>

#include "Subsystem.h"

struct FContentNodeHandle;
class LClass;

struct FContentNode
{
	friend class LAssetRegistry;
	friend struct FContentNodeHandle;
public:
	FContentNode(const std::filesystem::path& InPath, FContentNode* InParentNode);

	bool IsRoot() const { return Path.empty(); }
	bool IsValid() const;
	bool IsDirectory() const;

	std::string GetDisplayName() const;

	const std::filesystem::path& GetPath() const { return Path; }

private:
	const std::string GetKey() const;

	void operator=(FContentNode&& Other);

	std::filesystem::path Path;
	// TODO: This key could be a string_view, except std::filesystem::path isn't compatible. We'll need our own path type.
	std::map<std::string, FContentNode> Contents;

	bool bIsFirstNode = false;

	FContentNode* ParentNode = nullptr;
};

class LAssetRegistry : public LSubsystem
{
	friend struct FContentNodeHandle;
public:
	LAssetRegistry();

	virtual void Initialize() override;

	inline const std::unordered_set<std::filesystem::path>* GetAssetsByClass(LClass* Class) const
	{
		if (!Class || !ClassToAssetPathsMap.contains(Class))
		{
			return {};
		}

		return &ClassToAssetPathsMap.at(Class);
	}

	void CreateAsset(FContentNodeHandle& ParentNodeHandle, LClass* AssetClass);
	void CreateFolder(FContentNodeHandle& ParentNodeHandle, const std::string FolderName);
	bool RenameNode(FContentNodeHandle& ContentNode, const std::string NewName);
	void DeleteNode(const std::filesystem::path& Path) {}

	const FContentNode& GetRootNode() const { return RootNode; }

	bool IsRootPath(const std::filesystem::path& Path) const;

private:
	FContentNode RootNode;

private:
	void RegisterRootPath(const std::filesystem::path& Path);
	void RegisterPath(FContentNode* Parent, const std::filesystem::path& Path);
	void RegisterAsset(FContentNode* Parent, const std::filesystem::path& Path);

	void RegisterContentsToNode(FContentNode& Node);

	std::map<LClass*, std::unordered_set<std::filesystem::path>> ClassToAssetPathsMap;
};

