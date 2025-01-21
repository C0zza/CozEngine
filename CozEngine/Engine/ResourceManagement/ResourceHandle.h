#pragma once

#include <cassert>

#include "Resource.h"

template<typename T>
class LResourceHandle
{
public:
	LResourceHandle() : Resource{ nullptr } {};

	~LResourceHandle()
	{
		SetResource(nullptr);
	}

	void Init(LResource* i_Resource)
	{
		T* NewResource = dynamic_cast<T*>(i_Resource);
		SetResource(NewResource);
		assert(Resource);
	}

	T* Get() const
	{
		return Resource;
	}

	T* operator->()
	{
		return Resource;
	}

	LResourceHandle<T>& operator=(const LResourceHandle<T>& InResourceHandle)
	{
		SetResource(InResourceHandle.Resource);
		return *this;
	}

	// TODO: operator= create new reference before removing old (if valid) reference to prevent deleting and reloading

private:
	// T inherits from LResource
	T* Resource;

	void SetResource(T* NewResource)
	{
		if (Resource && NewResource != Resource)
		{
			Resource->DecrementRefs();
		}

		Resource = NewResource;
		
		if (Resource)
		{
			Resource->IncrementRefs();
		}
	}
};
