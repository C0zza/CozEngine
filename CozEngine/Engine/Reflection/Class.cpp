#include "stdafx.h"

#include "Class.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Misc/Logging.h"
#include "Reflection/ClassRegister.h"

void LClass::DrawEditorDetails(void* BaseAddress)
{
	if (DrawEditorFunc)
	{
		DrawEditorFunc(static_cast<uint8_t*>(BaseAddress));
	}
}

void LClass::SerializeAddress(uint8_t* Add, nlohmann::json& Json) const
{
	if (SerializeFunc)
	{
		SerializeFunc(Add, Json);
	}
}

void LClass::DeserializeAddress(uint8_t* Add, const nlohmann::json& Json) const
{
	if (DeserializeFunc)
	{
		DeserializeFunc(Add, Json);
	}
}

bool LClass::IsChildOf(const std::string& ClassName) const
{
	const LClass* Class = LClassRegister::Get(ClassName);
	assert(Class);
	return IsChildOf(Class);
}

bool LClass::IsChildOf(const LClass* Class) const
{
	assert(Class);

	LClass* ParentClass = GetParentClass();
	if (!ParentClass)
	{
		return false;
	}

	return Class == ParentClass ? true : ParentClass->IsChildOf(Class);
}
