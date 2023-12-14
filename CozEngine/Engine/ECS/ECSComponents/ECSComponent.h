#pragma once

#include "ECS/ECSDefinitions.h"

struct LECSComponent
{
	LEntityID EntityID = 0;

	virtual void Init() {}

	virtual void Destroy() {}
};

