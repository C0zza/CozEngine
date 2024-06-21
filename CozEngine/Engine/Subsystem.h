#pragma once

#include "ECS/ECSDefinitions.h"
#include "Misc/TypeIdGenerator.h"

class LSubsystem
{
	friend class LSubsystemCollection;
public:
	virtual bool HasRequiredSubsystems() const { return true; }

	virtual void PreTick() {};
	virtual void Tick() {};
	virtual void PostTick() {};

protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {}

	bool bTickable = false;
};

