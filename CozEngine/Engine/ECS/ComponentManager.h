#pragma once

#include "ECSDefinitions.h"

#include <memory>
#include <unordered_map>

#include "ComponentPool.h"

class LComponentManager
{
public:
	template<typename T>
	void RegisterComponent()
	{
		const char* TypeName = typeid(T).name();
		
		assert(ComponentTypes.find(TypeName) == ComponentTypes.end());

		ComponentTypes.insert({ TypeName, NextComponentType });
		ComponentPools.insert({ TypeName, std::make_shared<LComponentPool<T>>() });

		++NextComponentType;
	}

	template<typename T>
	LComponentType GetComponentType()
	{
		const char* TypeName = typeid(T).name();

		assert(ComponentTypes.find(TypeName) != ComponentTypes.end());

		return ComponentTypes[TypeName];
	}

	template<typename T>
	void AddComponent(LEntity Entity, T Component)
	{
		GetComponentPool<T>()->InsertData(Entity, Component);
	}

	template<typename T>
	void RemoveComponent(LEntity Entity)
	{
		GetComponentPool<T>()->RemoveData(Entity);
	}

	template<typename T>
	T& GetComponent(LEntity Entity)
	{
		return GetComponentPool<T>()->GetData(Entity);
	}

	void EntityDestroyed(LEntity Entity)
	{
		for (auto const& Pair : ComponentPools)
		{
			auto const& Component = Pair.second;
			Component->EntityDestroyed(Entity);
		}
	}

private:
	std::unordered_map<const char*, LComponentType> ComponentTypes;
	std::unordered_map<const char*, std::shared_ptr<IComponentPool>> ComponentPools;

	LComponentType NextComponentType;

	template<typename T>
	std::shared_ptr<LComponentPool<T>> GetComponentPool()
	{
		const char* TypeName = typeid(T).name();

		assert(ComponentTypes.find(TypeName) != ComponentTypes.end());

		return std::static_pointer_cast<LComponentPool<T>>(ComponentPools[TypeName]);
	}
};

