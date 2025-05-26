#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "Property.h"

class LClass;

class LClassUtilities
{
public:
	static void RegisterStaticClassProperties(LClass* Class,
												std::vector<LProperty>& Properties,
												const std::size_t Size,
												const std::size_t Alignment,
												const std::string& ClassName,
												const std::string& ParentClassName,
												std::function<void(uint8_t*)> DrawEditorFunc,
												std::function<void*()> CreateObjectFunc);

private:
	static std::unordered_map<std::string, std::unordered_set<std::string>> ParentToChildClassesMap;
};

#define MEMBER_OFFSET(Type, Member) offsetof(Type, Member)

