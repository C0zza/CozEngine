#include "SubsystemCollection.h"

#include "Subsystem.h"

LSubsystemCollection::~LSubsystemCollection()
{
	for (auto& Subsystem : Subsystems)
	{
		Subsystem.second->Deinitialize();
	}
}
