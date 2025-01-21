#include "SubsystemCollection.h"

#include "Globes.h"
#include "Subsystem.h"

LSubsystemCollection::LSubsystemCollection()
{
	TypeIdGenerator = new LTypeIdGenerator();
	RegisterSubsystem(TypeIdGenerator);

	RegisterSubsystem(new LTypeInstanceIdGenerator());
}

LSubsystemCollection::~LSubsystemCollection()
{
	std::map<LIDType, std::unique_ptr<LSubsystem>>::reverse_iterator it;
	for (it = Subsystems.rbegin(); it != Subsystems.rend(); ++it)
	{
		it->second->Deinitialize();
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
