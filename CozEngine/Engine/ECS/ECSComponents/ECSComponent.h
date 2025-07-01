#pragma once

#include "ECS/ECSDefinitions.h"
#include "json.hpp"
#include "Misc/FuncRegister.h"
#include "Reflection/Reflection.h"

using LComponentRegister = LFuncRegister<nlohmann::json>;
	
class LECS;

REFL_CLASS(Abstract)
struct LECSComponent
{
	LEntityID EntityID = 0;

	void InternalInit();

	virtual void Destroy() {}

protected:
	virtual void Init() {}

	LECS* ECS = nullptr;

	REFL_GENERATED_BODY(LECSComponent)
};

