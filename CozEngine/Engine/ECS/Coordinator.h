#pragma once

#include <memory>

#include "ComponentManager.h"
#include "ECSDefinitions.h"
#include "EntityManager.h"
#include "SystemManager.h"

class LCoordinator
{
public:
	void Init();

	LEntity CreateEntity();
	void DestroyEntity(LEntity Entity);

	template<typename T>
	void RegisterComponent()
	{
		ComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(LEntity Entity, T Component)
	{
		ComponentManager->AddComponent<T>(Entity, Component);

		auto Signature = EntityManager->GetSignature(Entity);
		Signature.set(ComponentManager->GetComponentType<T>(), true);
		EntityManager->SetSignature(Entity, Signature);

		SystemManager->EntitySignatureChanged(Entity, Signature);
	}

	template<typename T>
	void RemoveComponent(LEntity Entity)
	{
		ComponentManager->RemoveComponent<T>(Entity);

		auto Signature = EntityManager->GetSignature(Entity);
		Signature.set(ComponentManager->GetComponentType<T>(), false);
		EntityManager->SetSignature(Entity, Signature);

		SystemManager->EntitySignatureChanged(Entity, Signature);
	}

	template<typename T>
	T& GetComponent(LEntity Entity)
	{
		return ComponentManager->GetComponent<T>(Entity);
	}

	template<typename T>
	LComponentType GetComponentType()
	{
		return ComponentManager->GetComponentType<T>();
	}

	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return SystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(LSignature Signature)
	{
		SystemManager->SetSignature<T>(Signature);
	}

private:
	std::unique_ptr<LComponentManager> ComponentManager;
	std::unique_ptr<LEntityManager> EntityManager;
	std::unique_ptr<LSystemManager> SystemManager;
};

