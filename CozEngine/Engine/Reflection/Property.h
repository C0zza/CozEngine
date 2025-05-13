#pragma once

#include <string>

enum class EPropertyType
{
	Invalid,
	Int,
	Float,
	Bool
};

class LProperty
{
public:
	LProperty(const std::string& iPropertyName, const std::string& iPropertyType, const std::size_t iPropertyOffset)
		: PropertyName{ iPropertyName }, PropertyType{iPropertyType}, PropertyOffset{iPropertyOffset}
	{}

	const std::string& GetPropertyName() const { return PropertyName; }
	const std::string& GetPropertyType() const { return PropertyType; }
	std::size_t GetPropertyOffset() const { return PropertyOffset; }

private:
	std::string PropertyName = "";
	std::string PropertyType = "";
	std::size_t PropertyOffset = -1;
};

