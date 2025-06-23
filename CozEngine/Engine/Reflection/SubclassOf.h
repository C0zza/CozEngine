#pragma once

#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"

template<typename TBaseClass>
class TSubclassOf
{
public:
	LClass* GetBaseClass()
	{
		return TBaseClass::StaticClass();
	}

	const LClass* Get() const { return CurrentClass; }
	LClass* Get() { return CurrentClass; }
	bool IsValid() const { return CurrentClass; }

	bool operator==(const TSubclassOf<TBaseClass>& Other) const { return CurrentClass && CurrentClass == Other.CurrentClass; }
	void operator=(LClass* Class)
	{
		assert(GetBaseClass());
		assert(!Class || Class->IsChildOf<TBaseClass>());
		
		CurrentClass = Class;
	}

	friend void to_json(nlohmann::json& Json, const TSubclassOf<TBaseClass>& SubclassOf)
	{
		Json["Type"] = SubclassOf.IsValid() ? SubclassOf.Get()->GetTypeName().data() : "";
	}
	friend void from_json(const nlohmann::json& Json, TSubclassOf<TBaseClass>& SubclassOf)
	{
		const std::string TypeString = Json["Type"];

		if (TypeString.empty())
		{
			SubclassOf = nullptr;
			return;
		}

		LClass* Class = LClassRegister::Get(TypeString);
		SubclassOf = Class;
	}

	bool operator<(const TSubclassOf<TBaseClass>& Other) const
	{
		return CurrentClass < Other.CurrentClass;
	}

private:
	LClass* CurrentClass = nullptr;
};

