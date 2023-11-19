#pragma once

#include <unordered_map>
#include <vector>

#include "ECSDefinitions.h"

class LComponentSystemBase
{
public:
	virtual void Run() = 0;
	virtual void RemoveComponent(const LEntityID EntityID) = 0;
};

template<typename TComponentType>
class LComponentSystem : public LComponentSystemBase
{
public:
	virtual void Run() override
	{
		for (std::pair<LEntityID, TComponentType>& Component : Components)
		{
			RunComponent(Component.second);
		}
	}

	virtual void RunComponent(TComponentType& Component) = 0;

	void AddComponent(const LEntityID EntityID, const TComponentType& Component)
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			return;
		}

		EntityIdToComponentIndex[EntityID] = (unsigned int)Components.size(); // TODO: Check entity doesn't already exist
		Components.push_back({ EntityID,Component });
	}

	virtual void RemoveComponent(const LEntityID EntityID) final
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			const LEntityID IndexToRemove = EntityIdToComponentIndex[EntityID];
			const std::pair<LEntityID, TComponentType> LastComponent = Components[Components.size() - 1];

			Components[IndexToRemove] = LastComponent;
			Components.pop_back();
			EntityIdToComponentIndex[LastComponent.first] = IndexToRemove;

			EntityIdToComponentIndex.erase(EntityID);
		}
	}

	bool GetComponent(const LEntityID EntityID, TComponentType& Component)
	{
		if (!EntityIdToComponentIndex.contains(EntityID))
		{
			return false;
		}

		// TODO: Better to return reference? But less safe
		Component = Components[EntityIdToComponentIndex[EntityID]];
		return true;
	}

private:
	std::vector<std::pair<LEntityID,TComponentType>> Components;
	std::unordered_map<LEntityID, unsigned int> EntityIdToComponentIndex;
};
