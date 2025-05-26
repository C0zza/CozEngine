#include "stdafx.h"

#include "AssetRegistry.h"

#include <fstream>

#include "json.hpp"
#include "Misc/Logging.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"

void LAssetRegistry::Initialize()
{
	RegisterRootPath("Engine\\Content");
	RegisterRootPath("Game\\Content");
}

std::unordered_set<std::string_view> LAssetRegistry::GetAssetsByClass(LClass* Class) const
{
	if (!Class || !ClassToAssetPathsMap.contains(Class))
	{
		return {};
	}

	return ClassToAssetPathsMap.at(Class);
}

void LAssetRegistry::RegisterRootPath(const std::filesystem::path& Path)
{
	assert(!Path.empty());
	assert(std::filesystem::exists(Path));
	assert(std::filesystem::is_directory(Path));

	RegisterPath(nullptr, Path);
}

void LAssetRegistry::RegisterPath(FNode* Parent, const std::filesystem::path& Path)
{
	FNode& Node = Parent ? Parent->Contents.emplace_back() : RootNode.Contents.emplace_back();
	Node.Path = Path.string();

	for (const auto& Entry : std::filesystem::directory_iterator(Path))
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

void LAssetRegistry::RegisterAsset(FNode* Parent, const std::filesystem::path& Path)
{
	assert(Parent);
	assert(Path.string().ends_with(".casset"));

	FNode& Node = Parent->Contents.emplace_back();
	Node.Path = Path.string();

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
		Log(LLogLevel::ERROR, "LAssetRegistry::RegisterAsset - Invalid Type: " + TypeString + " in asset: " + Node.Path);
		return;
	}

	if (!ClassToAssetPathsMap.contains(TypeClass))
	{
		ClassToAssetPathsMap[TypeClass] = {};
	}

	ClassToAssetPathsMap.at(TypeClass).insert(Node.Path);
	LClass* ParentClass = TypeClass->GetParentClass();
	while (ParentClass)
	{
		if (!ClassToAssetPathsMap.contains(ParentClass))
		{
			ClassToAssetPathsMap[ParentClass] = {};
		}
		ClassToAssetPathsMap.at(ParentClass).insert(Node.Path);
		ParentClass = ParentClass->GetParentClass();
	}
}

bool LAssetRegistry::FNode::IsValid() const
{
	return Path.empty();
}

bool LAssetRegistry::FNode::IsDirectory() const
{
	return !Path.ends_with(".casset");
}
