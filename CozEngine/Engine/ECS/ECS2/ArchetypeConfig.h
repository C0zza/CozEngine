#pragma once

#include <set>
#include <string>

#include "ECS/ECSComponents/ECSComponent.h"
#include "json.hpp"
#include "Reflection/SubclassOf.h"
#include "Reflection/Reflection.h"
#include "ResourceManagement/Resource.h"

class LClass;

REFL_CLASS()
class LArchetypeConfig : public LResource
{
	REFL_GENERATED_BODY(LArchetypeConfig)
public:
	const std::set<LClass*>& GetComponentClasses() const { return ComponentClasses; }
	const std::set<TSubclassOf<LECSComponent>>& GetComponentSet() const { return ComponentSet; }
	std::set<TSubclassOf<LECSComponent>>& GetComponentSet() { return ComponentSet;  }

	bool Includes(const LArchetypeConfig& Config) const;

	bool operator<(const LArchetypeConfig& Other) const { return ComponentSet < Other.ComponentSet; }

private:
	REFL_PROP(Visible)
	std::set<TSubclassOf<LECSComponent>> ComponentSet;

	std::set<LClass*> ComponentClasses;
};

