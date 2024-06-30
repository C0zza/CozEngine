#pragma once

#include <cassert>
#include <map>

#include "ComponentSystem.h"
#include "ECSDefinitions.h"
#include "Misc/Logging.h"
#include "Misc/TypeIdGenerator.h"
#include "Subsystem.h"

class LECS : public LSubsystem
{
public:
	template<typename TComponentSystem, typename TComponentType>
	void AddComponentSystem()
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (ComponentSystems.contains(TypeID))
		{
			Log(LLogLevel::INFO, "LECS::AddComponentSystem - " + std::string(typeid(TComponentSystem).name()) + " has already been added.");
			return;
		}

		TComponentSystem* ComponentSystem = new TComponentSystem();
		ComponentSystems.insert({ TypeID, std::unique_ptr<TComponentSystem>(ComponentSystem) });
		ComponentSystems.at(TypeID)->InternalInit(this);
		

		if (ComponentSystems[TypeID]->GetIsTickable())
		{
			TickableComponentSystems.push_back(ComponentSystems[TypeID].get());
		}
	}

	// TODO: RemoveComponentSystem.

	template<typename TComponentType>
	LComponentSystemBase* GetComponentSystemFor()
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();
		if (!ComponentSystems.contains(TypeID))
		{
			Log(LLogLevel::INFO, "LECS::GetComponentSystemFor - Unable to get " + std::string(typeid(TComponentType).name()) + "'s component systems as there's none currently registered.");
			return nullptr;
		}

		return ComponentSystems[TypeID].get();
	}

	template<typename TComponentType, typename... TArgs>
	TComponentType* AddComponent(const LEntityID EntityID, TArgs&... Args)
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (!ComponentSystems.contains(TypeID))
		{
			Log(LLogLevel::INFO, "LECS::AddComponent - Unable to add " + std::string(typeid(TComponentType).name()) + " as there's no associated ComponentSystem.");
			return nullptr;
		}

		// meeeeeeeeeeeh
		LComponentSystem<TComponentType>* ComponentSystem = dynamic_cast<LComponentSystem<TComponentType>*>(ComponentSystems[TypeID].get());
		assert(ComponentSystem);
		// TODO: go over move semantics for the TComponentType(Args...)
		return ComponentSystem->AddComponent(EntityID, Args...);
	}

	template<typename TComponentType>
	void RemoveComponent(const LEntityID EntityID)
	{
		const LIDType TypeID = LUniqueTypeIdGenerator::GetTypeID<TComponentType>();

		if (!ComponentSystems.contains(TypeID))
		{
			Log(LLogLevel::INFO, "LECS::RemoveComponent - Unable to remove " + std::string(typeid(TComponentType).name()) + " as there's no associated ComponentSystem.");
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
			Log(LLogLevel::INFO, "LECS::GetComponent - Unable to get " + std::string(typeid(TComponentType).name()) + " as there's no associated ComponentSystem.");
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

	void Update();

private:
	std::map<LComponentTypeID, std::unique_ptr<LComponentSystemBase>> ComponentSystems;
	std::vector<LComponentSystemBase*> TickableComponentSystems;
};

