#pragma once

#include "SubsystemCollection.h"

class ISubsystem;

class LSystem
{
public:
	void Run();

	const LSubsystemCollection& GetSubsystems() { return Subsystems; }

private:
	LSubsystemCollection Subsystems;
};



