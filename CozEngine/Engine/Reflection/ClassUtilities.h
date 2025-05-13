#pragma once

#include "Property.h"

class LClass;

class LClassUtilities
{
public:
	static void RegisterStaticClassProperties(LClass* Class, std::vector<LProperty>& Properties);
};

#define MEMBER_OFFSET(Type, Member) offsetof(Type, Member)

