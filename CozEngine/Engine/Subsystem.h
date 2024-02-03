#pragma once

#include "ECS/ECSDefinitions.h"
#include "Misc/TypeIdGenerator.h"

class ISubsystem
{
	friend class LSubsystemCollection;
public:
	virtual bool HasRequiredSubsystems() const = 0;

protected:
	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;
};

template<typename T>
class LSubsystem : public ISubsystem
{
public:
	virtual bool HasRequiredSubsystems() const { return true; }

protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {}
};

