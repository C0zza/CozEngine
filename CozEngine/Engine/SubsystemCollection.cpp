#include "SubsystemCollection.h"

#include "Subsystem.h"

LSubsystemCollection::~LSubsystemCollection()
{
	for (auto& Subsystem : Subsystems)
	{
		Subsystem.second->Deinitialize();
	}
}

void LSubsystemCollection::ForEachSubsystem(std::function<void(LSubsystem*)>&& Func)
{
	for (std::pair<const LIDType, std::unique_ptr<LSubsystem>>& Pair : Subsystems)
	{
		if (Pair.second.get())
		{
			Func(Pair.second.get());
		}
	}
}
