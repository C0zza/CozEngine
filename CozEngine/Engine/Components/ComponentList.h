#pragma once

#include <memory>
#include <vector>

#include "Component.h"

class CObject;

class CComponentList
{
public:
	std::vector<std::unique_ptr<CComponent>>& GetComponents() { return Components; }

	template <typename TComponentType, typename... TInitArgs>
	inline TComponentType* AddComponent(CObject* Parent, TInitArgs... Args)
	{
		// Meh, expensive to create a raw pointer here. Can use make_ instead if we don't need the returned pointer too much.
		TComponentType* Component = new TComponentType(Args...);
		if (!Component)
		{
			return nullptr;
		}

		Components.emplace_back(Component);
		Components.back()->SetParent(Parent);

		return Component;
	}

	template <typename TComponentType>
	inline TComponentType* GetComponent()
	{
		for (std::unique_ptr<CComponent> CurrentComp : Components)
		{
			if (TComponentType* Component = dynamic_cast<TComponentType>(CurrentComp.get()))
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
			if (TComponentType* Component = dynamic_cast<TComponentType>(Components[i].get()))
			{
				Components.erase(i);
				return true;
			}
		}

		return false;
	}

private:
	std::vector<std::unique_ptr<CComponent>> Components;
};

