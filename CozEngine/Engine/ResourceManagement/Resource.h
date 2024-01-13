#pragma once

#include <string>

class LResource
{
public:
	virtual void Load() = 0;
	virtual void Unload() = 0;

	void IncrementRefs();
	void DecrementRefs();
	
	void SetAssetPath(const std::string& AssetPath);
	const std::string& GetAssetPath() const { return AssetPath; }

private:
	unsigned int RefCount = 0;
	std::string AssetPath;
};
