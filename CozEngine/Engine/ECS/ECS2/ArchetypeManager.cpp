#include "stdafx.h"

#include "ArchetypeManager.h"

#include "ECS/ECS2/Archetype.h"
#include "ECS/ECS2/ArchetypeConfig.h"
#include "ECS/ECS2/EntityArchetypeMap.h"
#include "ECS/Entity.h"
#include "Reflection/Class.h"
#include "WorldManager.h"

LArchetype* LArchetypeManager::GetArchetype(const FArchetypeSignature& Signature)
{
    return nullptr;
}

LEntityID LArchetypeManager::AddEntity(const LArchetypeConfig& Signature)
{
    if (!Archetypes.contains(Signature))
    {
        Archetypes.emplace(Signature, LArchetype(&Signature));
    }

    LArchetype& Archetype = Archetypes.at(Signature);

    LEntityID EntityID = 0;
    Archetype.AddEntity(EntityID);

    return EntityID;
}

void LArchetypeManager::RemoveEntity(const LEntityID EntityID)
{
    const LEntityArchetypeMap* EntityArchetypeMap = CSystem.GetSubsystems().GetSubsystem<LEntityArchetypeMap>();
    assert(EntityArchetypeMap);

    const FEntityArchetypeMapping* Mapping = EntityArchetypeMap->GetMapping(EntityID);

    if (!Mapping || !Mapping->Archetype)
    {
        return;
    }

    Mapping->Archetype->RemoveEntity(EntityID);
}

void LArchetypeManager::ForEachEntityComponent(const LEntityID EntityID, std::function<void(char* ComponentAddress, LClass* ComponentClass)> Func)
{
    const LEntityArchetypeMap* EntityArchetypeMap = CSystem.GetSubsystems().GetSubsystem<LEntityArchetypeMap>();

    const FEntityArchetypeMapping* Mapping = EntityArchetypeMap->GetMapping(EntityID);

    if (!Mapping || !Mapping->Archetype)
    {
        return;
    }

    for (LClass* Class : Mapping->Archetype->GetSignature().Classes)
    {
        char* Address = Mapping->Archetype->GetComponent(EntityID, Mapping->ChunkIndex, Mapping->ElementIndex, Class);
        Func(Address, Class);
    }
}

void LArchetypeManager::RunProcessors(const EEntityProcessorType Type)
{
    assert(bProcessorsInitialized);

    for (std::unique_ptr<LEntityProcessor>& Processor : Processors)
    {
        if (!Processor.get() || Processor->GetType() != Type)
        {
            continue;
        }

        const LArchetypeConfig& ProcessorConfig = Processor->GetConfig();

        std::vector<LArchetype*> MatchingArchetypes;

        for (auto& [Key, Value] : Archetypes)
        {
            const LArchetypeConfig& ArchetypeConfig = Key;
            LArchetype& Archetype = Value;

            // TODO: Should have config queries which are cached and can be dirtied when updates are needed.
            if (ArchetypeConfig.Includes(ProcessorConfig))
            {
                MatchingArchetypes.emplace_back(&Archetype);
            }
        }

        FEntityQueryResult QueryResult(MatchingArchetypes);

        Processor->Execute(QueryResult);
    }
}

void LArchetypeManager::InitializeProcessors()
{
    if (bProcessorsInitialized)
    {
        return;
    }

    for (std::unique_ptr<LEntityProcessor>& Processor : Processors)
    {
        if (Processor.get())
        {
            Processor->Initialize();
            assert(!Processor->GetConfig().GetComponentSet().empty());
            assert(Processor->GetType() != EEntityProcessorType::Invalid);
        }
    }

    bProcessorsInitialized = true;
}

std::vector<LEntityID> LArchetypeManager::GetAllEntityIDs()
{
    LEntityArchetypeMap* EntityArchetypeMap = CSystem.GetSubsystems().GetSubsystem<LEntityArchetypeMap>();
    if (!EntityArchetypeMap)
    {  
        return {};
    }

    std::vector<LEntityID> EntityIDs;
    EntityIDs.reserve(EntityArchetypeMap->EntityArchetypeMappings.size());

    for (const std::pair<LEntityID, FEntityArchetypeMapping>& Pair : EntityArchetypeMap->EntityArchetypeMappings)
    {
        EntityIDs.emplace_back(Pair.first);
    }

    return EntityIDs;
}

void LArchetypeManager::Initialize()
{
    LClass* ArchetypeProcessorClass = LEntityProcessor::StaticClass();

    const std::vector<LClass*> ChildClasses = ArchetypeProcessorClass->GetChildClasses();

    for (LClass* Class : ChildClasses)
    {
        assert(Class);

        LEntityProcessor* Processor = Class->CreateObject<LEntityProcessor>();
        Processors.emplace_back(std::unique_ptr<LEntityProcessor>(Processor));
    }
}

char* LArchetypeManager::GetEntityComponentAddress(const LEntityID EntityID, LClass* ComponentClass)
{
    const LEntityArchetypeMap* EntityArchetypeMap = CSystem.GetSubsystems().GetSubsystem<LEntityArchetypeMap>();

    const FEntityArchetypeMapping* Mapping = EntityArchetypeMap->GetMapping(EntityID);

    if (!Mapping || !Mapping->Archetype)
    {
        return nullptr;
    }

    return Mapping->Archetype->GetComponent(EntityID, Mapping->ChunkIndex, Mapping->ElementIndex, ComponentClass);
}
