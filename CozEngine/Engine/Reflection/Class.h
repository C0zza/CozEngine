#pragma once

#include <functional>
#include <map>
#include <vector>

#include "ClassUtilities.h"
#include "Property.h"

class LClass
{
	friend LClassUtilities;
public:
	void DrawEditorDetails(void* BaseAdd);

private:
	std::vector<LProperty> Properties;

	template<typename T>
	T* GetProperty(uint8_t* PropertyAddress) const;

	// TODO: Need something better. Can we deal with templated types? E.g. vector, map, etc.
	static std::map<std::string, std::function<void(LClass*, uint8_t*, const char*)>> DrawFuncs;
};

template<typename T>
inline T* LClass::GetProperty(uint8_t* PropertyAddress) const
{
	return reinterpret_cast<T*>(PropertyAddress);
}