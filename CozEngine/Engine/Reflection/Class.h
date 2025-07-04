#pragma once

#include <functional>
#include <map>
#include <vector>
#include <utility>

#include "ClassUtilities.h"
#include "json.hpp"
#include "Property.h"

class LClass
{
	friend LClassUtilities;
public:
	void DrawEditorDetails(void* BaseAdd);
	
	void SerializeAddress(uint8_t* Add, nlohmann::json& Json) const;
	void DeserializeAddress(uint8_t* Add, const nlohmann::json& Json) const;

	std::size_t GetByteSize() const { return ByteSize; }
	std::size_t GetByteAlignment() const { return ByteAlignment; }

	template<typename TParentClass>
	bool IsChildOf() const;
	bool IsChildOf(const std::string& ClassName) const;
	bool IsChildOf(const LClass* Class) const;

	template<typename T>
	T* CreateObject();
	
	void RunConstructor(uint8_t* Add);
	void RunDestructor(uint8_t* Add);

	const std::vector<LClass*>& GetChildClasses() const { return ChildClasses; }
	LClass* GetParentClass() const { return ParentClass; }

	std::string_view GetTypeName() const { return ClassName; }

protected:
	std::size_t ByteSize = 0;
	std::size_t ByteAlignment = 0;

private:
	std::vector<LProperty> Properties;

	template<typename T>
	T* GetProperty(uint8_t* PropertyAddress) const;

	std::vector<LClass*> ChildClasses;
	LClass* ParentClass = nullptr;

	std::string ClassName = "";

	std::function<void(uint8_t*)> ConstructAddressFunc;
	std::function<void(uint8_t*)> DestructAddressFunc;
	std::function<void*()> CreateObjectFunc;
	std::function<void(uint8_t*)> DrawEditorFunc;
	std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc;
	std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc;

	// TODO: Need something better. Can we deal with templated types? E.g. vector, map, etc.
	static std::map<std::string, std::function<void(LClass*, uint8_t*, const char*)>> DrawFuncs;
};

template<typename TClassType>
inline bool LClass::IsChildOf() const
{
	const LClass* QueriedClass = TClassType::StaticClass();
	assert(QueriedClass);
	return IsChildOf(QueriedClass);
}

template<typename T>
inline T* LClass::CreateObject()
{
	return CreateObjectFunc ? reinterpret_cast<T*>(CreateObjectFunc()) : nullptr;
}

template<typename T>
inline T* LClass::GetProperty(uint8_t* PropertyAddress) const
{
	return reinterpret_cast<T*>(PropertyAddress);
}