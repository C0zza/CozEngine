#pragma once

#include "ECS/ECSDefinitions.h"
#include "json.hpp"
#include "Misc/FuncRegister.h"

using LComponentRegister = LFuncRegister<nlohmann::json>;
	
class LECS;

struct LECSComponent
{
	LEntityID EntityID = 0;

	void InternalInit();

	virtual void Destroy() {}

protected:
	virtual void Init() {}

	LECS* ECS = nullptr;
};

