#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

#include "ECSDefinitions.h"
#include "Misc/Logging.h"

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
			Component.Destroy();
		}
	}

	virtual void Run() override
	{
		assert(IsTickable);
		for (TComponentType& Component : Components)
		{
			RunComponent(Component);
		}
	}

	template<typename... TArgs>
	TComponentType* AddComponent(const LEntityID EntityID, TArgs&... Args)
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			Log(LLogLevel::INFO, "Entity already has a " + std::string(typeid(TComponentType).name()) + ". Returning existing instance.");
			return GetComponent(EntityID);
		}

		unsigned int ComponentIndex = (unsigned int)Components.size();
		EntityIdToComponentIndex[EntityID] = ComponentIndex;
		Components.emplace_back(Args...);
		Components[ComponentIndex].EntityID = EntityID;
		Components[ComponentIndex].Init();

		return &Components[ComponentIndex];
	}

	virtual void RemoveComponent(const LEntityID EntityID) final
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			const LEntityID IndexToRemove = EntityIdToComponentIndex[EntityID];
			TComponentType& LastComponent = Components[Components.size() - 1];

			Components[IndexToRemove].Destroy();
			Components[IndexToRemove] = LastComponent;

			Components.pop_back();
			EntityIdToComponentIndex[LastComponent.EntityID] = IndexToRemove;

			EntityIdToComponentIndex.erase(EntityID);
		}
		else
		{
			Log(LLogLevel::INFO, "LComponentSystem::RemoveComponent - Entity does not have a " + std::string(typeid(TComponentType).name()) + ".");
		}
	}

	TComponentType* GetComponent(const LEntityID EntityID)
	{
		if (!EntityIdToComponentIndex.contains(EntityID))
		{
			Log(LLogLevel::INFO, "LComponentSystem::GetComponent - Entity does not have a " + std::string(typeid(TComponentType).name()) + ". Returning nullptr.");
			return nullptr;
		}

		return &Components[EntityIdToComponentIndex[EntityID]];
	}

private:
	virtual void RunComponent(TComponentType& Component) {};

	// TODO: reserve amount/ notify when reallocation of the vector occurs.
	std::vector<TComponentType> Components;
	std::unordered_map<LEntityID, unsigned int> EntityIdToComponentIndex;
};
