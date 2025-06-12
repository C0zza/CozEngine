#include "stdafx.h"

#include "AssetRegistry.h"

#include <fstream>

#include "Globes.h"
#include "json.hpp"
#include "Misc/Logging.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"

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
	: RootNode{FContentNode("", nullptr)}
{
}

void LAssetRegistry::Initialize()
{
	RootNode.Contents.emplace(EngineContent, FContentNode(EngineContent, nullptr));
	RegisterContentsToNode(RootNode.Contents.at(EngineContent));

	RootNode.Contents.emplace(ProjectContent, FContentNode(ProjectContent, nullptr));
	RegisterContentsToNode(RootNode.Contents.at(ProjectContent));
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
		Parent->Contents.emplace(Filename, FContentNode(Path, Parent));
	}
	else
	{
		RootNode.Contents.emplace(Filename, FContentNode(Path, Parent));
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

	Parent->Contents.emplace(Filename, FContentNode(Path, Parent));
	
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

FContentNode::FContentNode(const std::filesystem::path InPath, FContentNode* InParentNode)
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
}
