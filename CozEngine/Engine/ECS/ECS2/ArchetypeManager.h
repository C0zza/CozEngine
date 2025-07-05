#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <utility>

#include "ECS/ECS2/EntityProcessor.h"
#include "ECS/ECS2/EntityProcessorType.h"
#include "ECS/ECSDefinitions.h"
#include "ResourceManagement/ResourceHandle.h"
#include "Subsystem.h"

class LArchetype;
class LArchetypeConfig;
struct FArchetypeSignature;
class LClass;

class LArchetypeManager : public LSubsystem
{
public:
	LArchetype* GetArchetype(const FArchetypeSignature& Signature);
	LArchetype* GetArchetype(const LResourceHandle<LArchetypeConfig> ArchetypeConfig);
	LArchetype* GetUpdatedArchetype(const LArchetype* Archetype, const LClass* Type, const bool bAdded);

	LEntityID AddEntity(const LArchetypeConfig& Signature);
	void RemoveEntity(const LEntityID EntityID);

	template<typename TComponent>
	TComponent* GetEntityComponent(const LEntityID EntityID);

	void ForEachEntityComponent(const LEntityID EntityID, std::function<void(char* ComponentAddress, LClass* ComponentClass)> Func);

	void RunProcessors(const EEntityProcessorType Type);

	void InitializeProcessors();

	std::vector<LEntityID> GetAllEntityIDs();

protected:
	virtual void Initialize() override;

private:
	char* GetEntityComponentAddress(const LEntityID EntityID, LClass* ComponentClass);

	std::map<LArchetypeConfig, LArchetype> Archetypes;
	std::vector<std::unique_ptr<LEntityProcessor>> Processors;

	bool bProcessorsInitialized = false;
};

template<typename TComponent>
inline TComponent* LArchetypeManager::GetEntityComponent(const LEntityID EntityID)
{
	return reinterpret_cast<TComponent*>(GetEntityComponentAddress(EntityID, TComponent::StaticClass()));
}
