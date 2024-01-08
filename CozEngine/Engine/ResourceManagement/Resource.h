#pragma once

#include <cassert>
#include <string>

#include "Serializable.h"

class LResource : public LSavable
{
public:
	virtual void Load() = 0;
	virtual void Unload() = 0;

	void IncrementRefs();
	void DecrementRefs();
	
	void SetAssetPath(const std::string& AssetPath);
	const std::string& GetAssetPath() { return AssetPath; }

private:
	unsigned int RefCount = 0;
	std::string AssetPath;
};

template<typename T>
class LResourceHandle
{
public:
	LResourceHandle() {};
	LResourceHandle(LResource* i_Resource)
	{
		assert(i_Resource);
		Resource->IncrementRefs();
	}

	~LResourceHandle()
	{
		assert(Resource);
		Resource->DecrementRefs();
	}

	T* Get() const
	{ 
		return Resource; 
	}

	T* operator->()
	{
		return Resource;
	}

	// TODO: operator= create new reference before removing old (if valid) reference to prevent deleting and reloading

private:
	// T inherits from LResource
	T* Resource;
};
