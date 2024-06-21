#pragma once

#include "SubsystemCollection.h"

class LSystem
{
public:
	void Run();

	LSubsystemCollection& GetSubsystems() { return Subsystems; }

private:
	LSubsystemCollection Subsystems;
};



