#pragma once

#include <functional>

struct FContentNode;

struct FContentNodeHandle
{
public:
	FContentNodeHandle(const std::filesystem::path& InPath = "");

	void ForEachChildNode(std::function<void(FContentNodeHandle&)> Func);
	void StepOut();

	const FContentNode& GetNode();

	void Reset();

private:
	std::filesystem::path Path;

	class LAssetRegistry* AssetRegistry = nullptr;
};

