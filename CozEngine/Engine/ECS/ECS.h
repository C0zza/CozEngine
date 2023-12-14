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
	LECS();

	static LECS* Get() { return ECS; }

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
	TComponentType* AddComponent(const LEntityID EntityID, TArgs... Args)
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (!ComponentSystems.contains(TypeID))
		{
			return nullptr;
		}

		// meeeeeeeeeeeh
		LComponentSystem<TComponentType>* ComponentSystem = dynamic_cast<LComponentSystem<TComponentType>*>(ComponentSystems[TypeID].get());
		assert(ComponentSystem);
		// TODO: go over move semantics for the TComponentType(Args...)
		return ComponentSystem->AddComponent(EntityID, TComponentType(Args...));
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
	TComponentType* GetComponent(const LEntityID EntityID)
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (!ComponentSystems.contains(TypeID))
		{
			return nullptr;
		}

		LComponentSystem<TComponentType>* ComponentSystem = dynamic_cast<LComponentSystem<TComponentType>*>(ComponentSystems[TypeID].get());
		assert(ComponentSystem);
		return ComponentSystem->GetComponent(EntityID);
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

	static LECS* ECS;
};

