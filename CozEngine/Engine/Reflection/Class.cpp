#include "stdafx.h"

#include "Class.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Misc/Logging.h"

std::map<std::string, std::function<void(LClass*, uint8_t*, const char*)>> LClass::DrawFuncs
{
	{
		"int",
		[](LClass* Class, uint8_t* Add, const char* Name)
		{
			int* Property = Class->GetProperty<int>(Add);
			LImGuiPropertyDrawHelpers::DrawProperty(Name, *Property);
		},
	},
	{
		"float",
		[](LClass* Class, uint8_t* Add, const char* Name)
		{
			float* Property = Class->GetProperty<float>(Add);
			LImGuiPropertyDrawHelpers::DrawProperty(Name, *Property);
		},
	},
	{
		"bool",
		[](LClass* Class, uint8_t* Add, const char* Name)
		{
			bool* Property = Class->GetProperty<bool>(Add);
			LImGuiPropertyDrawHelpers::DrawProperty(Name, *Property);
		},
	},
};

void LClass::DrawEditorDetails(void* BaseAddress)
{
	for (const LProperty& Property : Properties)
	{
		uint8_t* PropertyAddress = reinterpret_cast<uint8_t*>(BaseAddress) + Property.GetPropertyOffset();
		const char* PropertyName = Property.GetPropertyName().c_str();

		if (!DrawFuncs.contains(Property.GetPropertyType()))
		{
			Log(LLogLevel::ERROR, "LClass::DrawEditorDetails - Property, " + Property.GetPropertyName() + ", has an invalid property type.");
			continue;
		}

		DrawFuncs[Property.GetPropertyType()](this, PropertyAddress, PropertyName);
	}
}
