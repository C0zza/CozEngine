#pragma once

#include "ECS/ECSDefinitions.h"

class LSubsystem
{
	friend class LSubsystemCollection;
public:
	virtual ~LSubsystem() {};

	virtual bool HasRequiredSubsystems() const { return true; }

protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {}
};

