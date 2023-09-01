#pragma once

#include <memory>
#include <vector>

#include "Component.h"

class CObject
{
public:
	template <typename TComponentType, typename... TInitArgs>
	inline TComponentType* AddComponent(TInitArgs... Args)
	{
		TComponentType* Component = new TComponentType(Args...);
		if (!Component)
		{
			return nullptr;
		}

		Components.push_back(Component);
		Component->SetParent(this);
		return Component;
	}

	template <typename TComponentType>
	inline TComponentType* GetComponent()
	{
		for (CComponent* CurrentComp : Components)
		{
			if (TComponentType* Component = dynamic_cast<TComponentType>(CurrentComp))
			{
				return Component;
			}
		}

		return nullptr;
	}

	template <typename TComponentType>
	inline bool RemoveComponent()
	{
		int ComponentIndex = -1;
		for (int i = 0; i < Components.size(); i++)
		{
			if (TComponentType* Component = dynamic_cast<TComponentType>(Components[i]))
			{
				Components.erase(i);
				return true;
			}
		}

		return false;
	}

private:
	std::vector<CComponent*> Components;
};
