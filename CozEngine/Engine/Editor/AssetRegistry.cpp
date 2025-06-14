#include "stdafx.h"

#include "AssetRegistry.h"

#include <filesystem>
#include <fstream>

#include "Editor/ContentNodeHandle.h"
#include "Globes.h"
#include "json.hpp"
#include "Misc/Logging.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "ResourceManagement/ResourceManager.h"

namespace
{
	// TODO: Could do with being a setting some where
	const std::string EngineContent = "Engine\\Content";
	const std::string ProjectContent = "Game\\Content";

	bool IsRootPath(const std::filesystem::path& Path)
	{
		return Path == EngineContent || Path == ProjectContent;
	}
}

LAssetRegistry::LAssetRegistry()
	: RootNode{FContentNode(std::string(""), nullptr)}
{
}

void LAssetRegistry::Initialize()
{
	RootNode.Contents.try_emplace(EngineContent, EngineContent, &RootNode);
	RegisterContentsToNode(RootNode.Contents.at(EngineContent));

	RootNode.Contents.try_emplace(ProjectContent, ProjectContent, &RootNode);
	RegisterContentsToNode(RootNode.Contents.at(ProjectContent));
}

void LAssetRegistry::CreateFolder(FContentNodeHandle& ParentNodeHandle, const std::string FolderName)
{
	if (!ParentNodeHandle.IsValid())
	{
		return;
	}

	FContentNode& ParentNode = ParentNodeHandle.GetMutableNode();

	// TODO: Add counter to FolderName and increment until available name is found
	if (ParentNode.Contents.contains(FolderName))
	{
		return;
	}

	std::filesystem::path FolderPath = ParentNode.GetPath().string() + "\\" + FolderName;

	if (!std::filesystem::create_directory(FolderPath))
	{
		return;
	}

	ParentNode.Contents.try_emplace(FolderName, FolderPath, &ParentNode);
}

bool LAssetRegistry::RenameNode(FContentNodeHandle& ContentNodeHandle, const std::string NewName)
{
	const FContentNode& Node = ContentNodeHandle.GetNode();
	if (Node.GetPath().stem().string() == NewName)
	{
		return true;
	}

	assert(Node.ParentNode);

	FContentNode* ParentNode = Node.ParentNode;
	if (!ParentNode->ParentNode)
	{
		// Renaming of first level nodes is not allowed.
		return true;
	}

	if (ParentNode->Contents.contains(NewName + (Node.IsDirectory() ? "" : ".casset")))
	{
		return false;
	}

	std::string NewPathString = Node.GetPath().string();
	NewPathString.erase(NewPathString.size() - Node.GetPath().filename().string().size());
	NewPathString.append(NewName);

	if (!Node.IsDirectory())
	{
		NewPathString.append(".casset");
	}

	std::filesystem::path NewPath = NewPathString;

	ParentNode->Contents.try_emplace(NewPath.filename().string(), NewPath, ParentNode);

	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	assert(ResourceManager);

	ResourceManager->OnAssetPathUpdated(Node.GetPath(), NewPath);

	std::filesystem::rename(Node.GetPath(), NewPath);

	ParentNode->Contents.erase(Node.GetPath().filename().string());

	return true;
}

bool LAssetRegistry::IsRootPath(const std::filesystem::path& Path) const
{
	return Path == ProjectContent || Path == EngineContent;
}

void LAssetRegistry::RegisterRootPath(const std::filesystem::path& Path)
{
	assert(!Path.empty());
	assert(std::filesystem::exists(Path));
	assert(std::filesystem::is_directory(Path));

	RegisterPath(nullptr, Path);
}

void LAssetRegistry::RegisterPath(FContentNode* Parent, const std::filesystem::path& Path)
{
	const std::string Filename = Path.filename().string();

	if (Parent)
	{
		Parent->Contents.try_emplace(Filename, Path, Parent);
	}
	else
	{
		RootNode.Contents.try_emplace(Filename, Path, Parent);
		Parent = &RootNode;
	}

	FContentNode& Node = Parent->Contents.at(Filename);
	Node.Path = Path.string();
	Node.ParentNode = Parent;

	RegisterContentsToNode(Node);
}

void LAssetRegistry::RegisterAsset(FContentNode* Parent, const std::filesystem::path& Path)
{
	assert(Parent);
	assert(Path.string().ends_with(".casset"));

	const std::string Filename = Path.filename().string();

	Parent->Contents.try_emplace(Filename, Path, Parent);
	
	FContentNode& Node = Parent->Contents.at(Filename);

	std::ifstream File(Path);
	assert(File.is_open());

	nlohmann::json Json;
	File >> Json;

	assert(Json.contains("Type"));
	assert(Json["Type"].is_string());

	const std::string TypeString = Json["Type"];
	LClass* TypeClass = LClassRegister::Get(TypeString);
	if (!TypeClass)
	{
		Log(LLogLevel::ERROR, "LAssetRegistry::RegisterAsset - Invalid Type: " + TypeString + " in asset: " + Node.GetPath().string());
		return;
	}

	if (!ClassToAssetPathsMap.contains(TypeClass))
	{
		ClassToAssetPathsMap[TypeClass] = {};
	}

	ClassToAssetPathsMap.at(TypeClass).insert(Node.GetPath());
	LClass* ParentClass = TypeClass->GetParentClass();
	while (ParentClass)
	{
		if (!ClassToAssetPathsMap.contains(ParentClass))
		{
			ClassToAssetPathsMap[ParentClass] = {};
		}
		ClassToAssetPathsMap.at(ParentClass).insert(Node.GetPath());
		ParentClass = ParentClass->GetParentClass();
	}
}

void LAssetRegistry::RegisterContentsToNode(FContentNode& Node)
{
	for (const auto& Entry : std::filesystem::directory_iterator(Node.GetPath()))
	{
		if (Entry.is_directory())
		{
			RegisterPath(&Node, Entry.path());
		}
		else if (Entry.path().extension() == ".casset")
		{
			RegisterAsset(&Node, Entry.path());
		}
		else
		{
			Log(LLogLevel::WARNING, "LAssetRegistry::RegisterRootPath - Unexpected filetype found in content folder.");
		}
	}
}

FContentNode::FContentNode(const std::filesystem::path& InPath, FContentNode* InParentNode)
	: Path{ InPath }, ParentNode{ InParentNode }, bIsFirstNode{ IsRootPath(InPath) }
{
}

bool FContentNode::IsValid() const
{
	return Path.empty();
}

bool FContentNode::IsDirectory() const
{
	return !Path.string().ends_with(".casset");
}

std::string FContentNode::GetDisplayName() const
{
	if (Path == EngineContent || Path == ProjectContent)
	{
		return Path.parent_path().string();
	}
	else
	{
		return Path.stem().string();
	}
}

const std::string FContentNode::GetKey() const
{
	if (bIsFirstNode)
	{
		return Path.string();
	}
	else
	{
		return Path.stem().string();
	}
}

void FContentNode::operator=(FContentNode&& Other)
{
	Path = Other.Path;
	bIsFirstNode = Other.bIsFirstNode;
	ParentNode = Other.ParentNode;
	Contents = std::move(Other.Contents);
}
