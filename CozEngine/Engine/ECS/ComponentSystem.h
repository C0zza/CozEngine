#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

#include "ECSDefinitions.h"

class LECS;

class LComponentSystemBase
{
public:
	static void SetECS(LECS* i_ECS);

protected:
	bool IsTickable = false;

	static LECS* ECS;

public:
	bool GetIsTickable() const { return IsTickable; }

	virtual void Run() = 0;
	virtual void RemoveComponent(const LEntityID EntityID) = 0;
};

template<typename TComponentType>
class LComponentSystem : public LComponentSystemBase
{
public:
	virtual void Run() override
	{
		assert(IsTickable);
		for (std::pair<LEntityID, TComponentType>& Component : Components)
		{
			RunComponent(Component.second);
		}
	}

	virtual void RunComponent(TComponentType& Component) {};

	void AddComponent(const LEntityID EntityID, const TComponentType& Component)
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			return;
		}

		unsigned int ComponentIndex = (unsigned int)Components.size();
		EntityIdToComponentIndex[EntityID] = ComponentIndex; // TODO: Check entity doesn't already exist
		Components.push_back({ EntityID,Component });
		Components[ComponentIndex].second.EntityID = EntityID;
	}

	virtual void RemoveComponent(const LEntityID EntityID) final
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			const LEntityID IndexToRemove = EntityIdToComponentIndex[EntityID];
			std::pair<LEntityID, TComponentType>& LastComponent = Components[Components.size() - 1];

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
		Component = Components[EntityIdToComponentIndex[EntityID]].second;
		return true;
	}

private:
	std::vector<std::pair<LEntityID,TComponentType>> Components;
	std::unordered_map<LEntityID, unsigned int> EntityIdToComponentIndex;
};
