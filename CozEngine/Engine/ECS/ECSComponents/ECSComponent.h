#pragma once

#include "ECS/ECSDefinitions.h"

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

