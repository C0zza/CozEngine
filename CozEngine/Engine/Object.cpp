#include "Object.h"

void CObject::TickComponents()
{
	for (std::unique_ptr<CComponent>& Component : Components.GetComponents())
	{
		if (Component.get())
		{
			Component->Tick();
		}
	}
}
