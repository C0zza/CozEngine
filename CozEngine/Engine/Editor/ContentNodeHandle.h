#pragma once

#include <functional>

struct FContentNode;

struct FContentNodeHandle
{
	friend class LAssetRegistry;
public:
	FContentNodeHandle(const std::filesystem::path& InPath = "");

	void ForEachChildNode(std::function<void(FContentNodeHandle&)> Func);
	void StepOut();

	const FContentNode& GetNode() const;
	void Validate();

	bool IsValid() const;
	void Reset();

	bool operator==(const FContentNodeHandle& Other) const { return Path == Other.Path; }

private:
	FContentNode& GetMutableNode() const;

	std::filesystem::path Path;

	class LAssetRegistry* AssetRegistry = nullptr;
};

