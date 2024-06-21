#pragma once

#include "Subsystem.h"

class LImGuiSubsystem : public LSubsystem
{
public:
	virtual bool HasRequiredSubsystems() const override;

	virtual void PreTick() override;
	virtual void PostTick() override;

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;
};

