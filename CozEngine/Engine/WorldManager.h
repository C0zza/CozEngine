#pragma once

#include <memory>

#include "ECS/EntityManagement/World.h"
#include "Subsystem.h"

class LWorldManager : public LSubsystem
{
public:
	virtual void Initialize() override final;

private:
	std::unique_ptr<LWorld> ActiveWorld;
};

