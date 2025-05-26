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
		: Name{ iPropertyName }, Type{iPropertyType}, Offset{iPropertyOffset}
	{}

	const std::string& GetPropertyName() const { return Name; }
	const std::string& GetPropertyType() const { return Type; }
	std::size_t GetPropertyOffset() const { return Offset; }

private:
	std::string Name = "";
	std::string Type = "";
	std::size_t Offset = -1;
};

