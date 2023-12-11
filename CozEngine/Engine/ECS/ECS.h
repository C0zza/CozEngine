#pragma once

#include <cassert>
#include <memory>
#include <map>

#include "ComponentSystem.h"
#include "ECSDefinitions.h"
#include "Misc/TypeIdGenerator.h"

class LECS
{
public:
	void RunComponentSystems();

	template<typename TComponentSystem, typename TComponentType>
	void AddComponentSystem()
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (ComponentSystems.contains(TypeID))
		{
			return;
		}

		ComponentSystems.insert({ TypeID, std::make_unique<TComponentSystem>() });

		if (ComponentSystems[TypeID]->GetIsTickable())
		{
			TickableComponentSystems.push_back(ComponentSystems[TypeID].get());
		}
	}

	// TODO: RemoveComponentSystem. Manage ComponentSystems and TickableComponentSystems

	template<typename TComponentType, typename... TArgs>
	void AddComponent(const LEntityID EntityID, TArgs... Args)
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (!ComponentSystems.contains(TypeID))
		{
			return;
		}

		// meeeeeeeeeeeh
		LComponentSystem<TComponentType>* ComponentSystem = dynamic_cast<LComponentSystem<TComponentType>*>(ComponentSystems[TypeID].get());
		assert(ComponentSystem);
		// TODO: go over move semantics for the TComponentType(Args...)
		ComponentSystem->AddComponent(EntityID, TComponentType(Args...));
	}

	template<typename TComponentType>
	void RemoveComponent(const LEntityID EntityID)
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (!ComponentSystems.contains(TypeID))
		{
			// TODO: Add logging system. Could just be std::cout stuff
			return;
		}

		ComponentSystems[TypeID]->RemoveComponent(EntityID);
	}

	template<typename TComponentType>
	bool GetComponent(const LEntityID EntityID, TComponentType& Component)
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (!ComponentSystems.contains(TypeID))
		{
			return false;
		}

		LComponentSystem<TComponentType>* ComponentSystem = dynamic_cast<LComponentSystem<TComponentType>>(ComponentSystems[TypeID].get());
		assert(ComponentSystem);
		return ComponentSystem->GetComponent(EntityID, Component);
	}

	void RemoveEntity(const LEntityID EntityID)
	{
		for (auto& ComponentSystem : ComponentSystems)
		{
			ComponentSystem.second->RemoveComponent(EntityID);
		}
	}

private:
	std::map<LComponentTypeID, std::unique_ptr<LComponentSystemBase>> ComponentSystems;
	std::vector<LComponentSystemBase*> TickableComponentSystems;
};

