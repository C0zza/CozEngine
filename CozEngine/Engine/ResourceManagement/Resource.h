#pragma once

#include <string>

using FAssetPath = std::string;

class LResource
{
public:
	virtual ~LResource() {}

	virtual void Load() = 0;
	virtual void Unload() = 0;

	void IncrementRefs();
	void DecrementRefs();
	
	void SetAssetPath(const FAssetPath& AssetPath);
	const FAssetPath& GetAssetPath() const { return AssetPath; }

private:
	unsigned int RefCount = 0;
	FAssetPath AssetPath;
};
