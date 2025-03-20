#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

#include "ECS/ECSComponents/ECSComponent.h"
#include "ECSDefinitions.h"
#include "Misc/Logging.h"

class LECS;

enum class EComponentSystemType
{
	Default,
	Ticker,
	Renderer
};

class LComponentSystemBase
{
	friend class LECS;
public:
	virtual ~LComponentSystemBase() {}

	virtual EComponentSystemType GetComponentSystemType() const { return EComponentSystemType::Default; }

	virtual bool ContainsComponent(const LEntityID EntityID) const = 0;

	virtual void RemoveComponent(const LEntityID EntityID) = 0;

	virtual bool GetSerializedComponent(const LEntityID EntityID, nlohmann::json& J) const = 0;
	virtual void DeserializeEntityComponentID(const LEntityID EntityID, const nlohmann::json& J) = 0;
	virtual const char* GetComponentName() const = 0;
protected:
	virtual void Init() = 0;

private:
	virtual void InternalRun() = 0;
	virtual void InternalInit(LECS* i_ECS) = 0;
};

template<typename TComponentType>
class LComponentSystem : public LComponentSystemBase
{
public:
	// TODO: As expected std::vector is unsafe with our AddComponent and GetComponent returning pointers to components.
	// Either return references which will be safer (but not completely) or change to array.
	LComponentSystem() { Components.reserve(100); }
	virtual ~LComponentSystem()
	{
		for (auto& Component : Components)
		{
			Component.Destroy();
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
		Components[ComponentIndex].InternalInit();

		OnComponentAdded(Components[ComponentIndex]);

		return &Components[ComponentIndex];
	}

	virtual void RemoveComponent(const LEntityID EntityID) final
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			const LEntityID IndexToRemove = EntityIdToComponentIndex[EntityID];
			TComponentType& LastComponent = Components[Components.size() - 1];

			Components[IndexToRemove].Destroy();

			if (&Components[IndexToRemove] != &LastComponent)
			{
				Components[IndexToRemove] = LastComponent;
			}

			Components.pop_back();
			EntityIdToComponentIndex[LastComponent.EntityID] = IndexToRemove;

			EntityIdToComponentIndex.erase(EntityID);
		}
		else
		{
			Log(LLogLevel::INFO, "LComponentSystem::RemoveComponent - Entity does not have a " + std::string(typeid(TComponentType).name()) + ".");
		}
	}

	virtual void DeserializeComponent(TComponentType& Component, const nlohmann::json& J) = 0;

	virtual void DeserializeEntityComponentID(const LEntityID EntityID, const nlohmann::json& J) final
	{
		if (ContainsComponent(EntityID))
		{
			DeserializeComponent(*GetComponent(EntityID), J);
		}
	}

	virtual bool ContainsComponent(const LEntityID EntityID) const final
	{
		return EntityIdToComponentIndex.contains(EntityID);
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

	const TComponentType* GetComponent(const LEntityID EntityID) const
	{
		return GetComponent(EntityID);
	}

protected:
	virtual void Init() override {}
	virtual void OnComponentAdded(TComponentType& Component) {};
	virtual void OnComponentRemoved(TComponentType& Component) {};

	std::vector<TComponentType>& GetComponents()
	{
		return Components;
	}

	virtual void GetSerializedComponent(const TComponentType& Component, nlohmann::json& J) const = 0;

	virtual const char* GetComponentName() const = 0;

	LECS* ECS = nullptr;

private:
	virtual void RunComponent(TComponentType& Component) {};
	virtual void PreRun() {};
	virtual void PostRun() {};

	virtual void InternalRun() final
	{
		assert(GetComponentSystemType() == EComponentSystemType::Ticker || GetComponentSystemType() == EComponentSystemType::Renderer);
		PreRun();
		for (TComponentType& Component : Components)
		{
			RunComponent(Component);
		}
		PostRun();
	}

	virtual void InternalInit(LECS* i_ECS) final
	{
		ECS = i_ECS;
		Init();
	}

	virtual bool GetSerializedComponent(const LEntityID EntityID, nlohmann::json& J) const override
	{
		if (EntityIdToComponentIndex.contains(EntityID))
		{
			const TComponentType& Component = Components[EntityIdToComponentIndex.at(EntityID)];
			GetSerializedComponent(Component, J);
			return true;
		}
		else
		{
			return false;
		}
	}

	// TODO: reserve amount/ notify when reallocation of the vector occurs.
	std::vector<TComponentType> Components;
	std::unordered_map<LEntityID, unsigned int> EntityIdToComponentIndex;
};