#include "stdafx.h"

#include "ClassUtilities.h"

#include "Class.h"
#include "ClassRegister.h"
#include "Misc/Logging.h"

std::unordered_map<std::string, std::unordered_set<std::string>> LClassUtilities::ParentToChildClassesMap = {};

void LClassUtilities::RegisterStaticClassProperties(LClass* Class,
													std::vector<LProperty>& Properties,
													const std::size_t Size,
													const std::size_t Alignment,
													const std::string& ClassName,
													const std::string& ParentClassName,
													std::function<void(uint8_t*)> DrawEditorFunc,
													std::function<void* ()> CreateObjectFunc,
													std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc,
													std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc,
													std::function<void(uint8_t*)> ConstructAddressFunc,
													std::function<void(uint8_t*)> DestructAddressFunc)
{
	if (!Class)
	{
		Log(LLogLevel::ERROR, "LClassUtilities::RegisterStaticClassProperties - Invalid Class.");
		return;
	}

	Class->Properties = std::move(Properties);

	Class->ByteSize = Size;
	Class->ByteAlignment = Alignment;

	Class->ClassName = ClassName;

	Class->DrawEditorFunc = DrawEditorFunc;
	Class->CreateObjectFunc = CreateObjectFunc;
	Class->SerializeFunc = SerializeFunc;
	Class->DeserializeFunc = DeserializeFunc;
	Class->ConstructAddressFunc = ConstructAddressFunc;
	Class->DestructAddressFunc = DestructAddressFunc;

	if (ParentToChildClassesMap.contains(Class->GetTypeName().data()))
	{
		for (const std::string& ChildClassString : ParentToChildClassesMap.at(Class->GetTypeName().data()))
		{
			LClass* ChildClass = LClassRegister::Get(ChildClassString);
			Class->ChildClasses.emplace_back(ChildClass);
			ChildClass->ParentClass = Class;
		}
	}
	else
	{
		ParentToChildClassesMap[Class->GetTypeName().data()] = {};
	}

	if (!ParentClassName.empty())
	{
		if (ParentToChildClassesMap.contains(ParentClassName))
		{
			bool bIsResourceClass = ParentClassName == "LResource";

			Class->ParentClass = LClassRegister::Get(ParentClassName);

			// Parent class may not be valid since generated files currently don't check whether it's reflected
			if (Class->ParentClass)
			{
				Class->ParentClass->ChildClasses.emplace_back(Class);
			}

			ParentToChildClassesMap[ParentClassName].emplace(ClassName);
		}
		else
		{
			ParentToChildClassesMap[ParentClassName] = { static_cast<std::string>(Class->GetTypeName()) };
		}
	}
}
