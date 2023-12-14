#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

#include "ECSDefinitions.h"

class LECS;

class LComponentSystemBase
{
protected:
	bool IsTickable = false;

public:
	bool GetIsTickable() const { return IsTickable; }

	virtual void Run() = 0;
	virtual void RemoveComponent(const LEntityID EntityID) = 0;
};

template<typename TComponentType>
class LComponentSystem : public LComponentSystemBase
{
public:
	// TODO: As expected std::vector is unsafe with our AddComponent and GetComponent returning pointers to components.
	// Either return references which will be safer (but not completely) or change to array.
	LComponentSystem() { Components.reserve(100); }
	~LComponentSystem()
	{
		for (auto& Component : Components)
		{
			Component.second.Destroy();
		}
	}

	virtual void Run() override
	{
		assert(IsTickable);
		for (std::pair<LEntityID, TComponentType>& Component : Components)
		{
			RunComponent(Component.second);
		}
	}

	TComponentType* AddComponent(const LEntityID EntityID, const TComponentType& Component)
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			return GetComponent(EntityID);
		}

		unsigned int ComponentIndex = (unsigned int)Components.size();
		EntityIdToComponentIndex[EntityID] = ComponentIndex; // TODO: Check entity doesn't already exist
		Components.push_back({ EntityID,Component });
		Components[ComponentIndex].second.EntityID = EntityID;
		Components[ComponentIndex].second.Init();

		return &Components[ComponentIndex].second;
	}

	virtual void RemoveComponent(const LEntityID EntityID) final
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			const LEntityID IndexToRemove = EntityIdToComponentIndex[EntityID];
			std::pair<LEntityID, TComponentType>& LastComponent = Components[Components.size() - 1];

			Components[IndexToRemove].second.Destroy();
			Components[IndexToRemove] = LastComponent;

			Components.pop_back();
			EntityIdToComponentIndex[LastComponent.first] = IndexToRemove;

			EntityIdToComponentIndex.erase(EntityID);
		}
	}

	TComponentType* GetComponent(const LEntityID EntityID)
	{
		if (!EntityIdToComponentIndex.contains(EntityID))
		{
			return nullptr;
		}

		return &Components[EntityIdToComponentIndex[EntityID]].second;
	}

private:
	virtual void RunComponent(TComponentType& Component) {};

	// TODO: reserve amount/ notify when reallocation of the vector occurs.
	std::vector<std::pair<LEntityID,TComponentType>> Components;
	std::unordered_map<LEntityID, unsigned int> EntityIdToComponentIndex;
};
