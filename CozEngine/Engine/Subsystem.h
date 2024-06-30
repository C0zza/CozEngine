#pragma once

#include "ECS/ECSDefinitions.h"
#include "Misc/TypeIdGenerator.h"

class LSubsystem
{
	friend class LSubsystemCollection;
public:
	virtual bool HasRequiredSubsystems() const { return true; }

protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {}
};

