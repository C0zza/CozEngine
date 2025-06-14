#include "stdafx.h"

#include "ContentNodeHandle.h"

#include "Editor/AssetRegistry.h"
#include "Globes.h"
#include "Misc/Logging.h"

namespace
{
	// TODO: Could do with being a setting some where
	const std::string EngineContent = "Engine\\Content";
	const std::string ProjectContent = "Game\\Content";
}


FContentNodeHandle::FContentNodeHandle(const std::filesystem::path& InPath)
{
	AssetRegistry = CSystem.GetSubsystems().GetSubsystem<LAssetRegistry>();
	assert(AssetRegistry);

	Path = InPath;
}

void FContentNodeHandle::ForEachChildNode(std::function<void(FContentNodeHandle&)> Func)
{
	const FContentNode& CurrentNode = GetNode();

	for (const auto& Pair : CurrentNode.Contents)
	{
		FContentNodeHandle Handle(Pair.second.GetPath());
		Func(Handle);
	}
}

void FContentNodeHandle::StepOut()
{
	if (Path.empty())
	{
		Log(LLogLevel::INFO, "FContentNodeHandle::StepOut - Already at root. Unable to step further out.");
		return;
	}

	if (AssetRegistry->GetRootNode().Contents.contains(Path.string()))
	{
		Path.clear();
	}
	else
	{
		Path = Path.parent_path();
	}
}

const FContentNode& FContentNodeHandle::GetNode() const
{
	const FContentNode& Node = GetMutableNode();
	return Node;
}

void FContentNodeHandle::Validate()
{
	if (!IsValid())
	{
		Path.clear();
	}
}

bool FContentNodeHandle::IsValid() const
{
	if (Path.empty())
	{
		return false;
	}

	const std::filesystem::path PathCopy = Path;
	return GetNode().GetPath() == Path;
}

void FContentNodeHandle::Reset()
{
	Path.clear();
}

FContentNode& FContentNodeHandle::GetMutableNode() const
{
	FContentNode& RootNode = AssetRegistry->RootNode;

	if (Path.empty())
	{
		return RootNode;
	}

	if (AssetRegistry->IsRootPath(Path))
	{
		return RootNode.Contents.at(Path.string());
	}

	std::string RootString;
	std::string PathString = Path.string();
	for (const std::pair<const std::string, FContentNode>& Pair : RootNode.Contents)
	{
		if (PathString.starts_with(Pair.first + "\\"))
		{
			RootString = Pair.first;
			break;
		}
	}

	bool bTemp = !RootString.empty();
	assert(bTemp);

	PathString.erase(0, RootString.size() + 1);

	FContentNode* CurrentNode = &RootNode.Contents.at(RootString);

	std::filesystem::path RemovedRootPath = PathString;
	for (const std::filesystem::path& PathSegment : RemovedRootPath)
	{
		if (!CurrentNode->Contents.contains(PathSegment.string()))
		{
			Log(LLogLevel::INFO, "FContentNodeHandle::GetNode - Failed to find " + Path.string() + ". Returning to " + CurrentNode->GetPath().string());
			break;
		}

		CurrentNode = &CurrentNode->Contents.at(PathSegment.string());
	}

	assert(CurrentNode);
	return *CurrentNode;
}
