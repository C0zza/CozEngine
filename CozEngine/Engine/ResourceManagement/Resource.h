#pragma once

#include <string>

#include "Reflection/Reflection.h"

using FAssetPath = std::string;

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
	const FAssetPath& GetAssetPath() const { return AssetPath; }

private:
	unsigned int RefCount = 0;
	FAssetPath AssetPath;
};
