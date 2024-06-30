#pragma once

#include "Subsystem.h"

class LImGuiSubsystem : public LSubsystem
{
public:
	virtual bool HasRequiredSubsystems() const override;

	void NewFrame();
	void Render();

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;
};

