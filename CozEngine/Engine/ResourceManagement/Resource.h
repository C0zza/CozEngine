#pragma once

#include <filesystem>

#include "Reflection/Reflection.h"
#include "ResourceManagement/AssetPath.h"

REFL_CLASS()
class LResource
{
	REFL_GENERATED_BODY(LResource)
public:
	virtual ~LResource() {}

	virtual void Load() {};
	virtual void Unload() {};

	void IncrementRefs();
	void DecrementRefs();
	
	void SetAssetPath(const FAssetPath& AssetPath);
	const std::string GetAssetPath() const { return AssetPath.string(); }

private:
	unsigned int RefCount = 0;
	FAssetPath AssetPath;
};
