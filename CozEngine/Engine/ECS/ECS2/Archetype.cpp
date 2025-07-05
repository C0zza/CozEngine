#include "stdafx.h"

#include "Archetype.h"

#include <cstdlib>

#include "ECS/ECS2/ArchetypeConfig.h"
#include "ECS/ECS2/EntityArchetypeMap.h"
#include "Globes.h"
#include "Misc/Logging.h"
#include "Reflection/Class.h"

#define CHUNK_SIZE 16384

static constexpr std::size_t AlignUp(const std::size_t Value, const std::size_t Alignment)
{
    return (Value + Alignment - 1) & ~(Alignment - 1);
}

LArchetype::LArchetype(const LArchetypeConfig* Config)
{
    FArchetypeSignature Signature;
    for (const TSubclassOf<LECSComponent>& ECSComponentClass : Config->GetComponentSet())
    {
        LClass* Class = ECSComponentClass.Get();
        if (Class)
        {
            Signature.Classes.emplace(Class);
        }
    }

    Initialize(Signature);
}

LArchetype::LArchetype(const FArchetypeSignature& Signature)
{    
    Initialize(Signature);
}

LArchetype::~LArchetype()
{
    for (FEntityChunk& EntityChunk : EntityChunks)
    {
        // TODO: Can probably rewrite this so we're just incrementing the ComponentAddress based on EntityCount, rather
        // than calculating it from scratch every iteration.
        for (int i = 0; i < EntityChunk.EntityCount; ++i)
        {
            for (std::pair<LClass* const, FComponentInfo>& Pair : ClassToComponentInfoMap)
            {
                LClass* Class = Pair.first;
                const FComponentInfo& ComponentInfo = Pair.second;

                char* ComponentAddress = GetComponentAddress(Class, EntityChunk, i);
                Class->RunDestructor(reinterpret_cast<uint8_t*>(ComponentAddress));
            }
        }

        _aligned_free(EntityChunk.Address);
        std::memset(EntityChunk.Address, 0, StrictChunkSize);
    }
}

bool LArchetype::AddEntity(LEntityID& EntityID)
{
    if (MaxAlignment > 0 && GetNewChunksNeeded(1))
    {
        void* ChunkAddress = _aligned_malloc(StrictChunkSize, MaxAlignment);
        if (!ChunkAddress)
        {
            Log(LLogLevel::ERROR, "LArchetype::RegisterChunks - Failed to allocate ChunkAddress");
            return false;
        }

        std::memset(ChunkAddress, 0, StrictChunkSize);

        EntityChunks.emplace_back(static_cast<char*>(ChunkAddress));
    }

    if (EntityChunks.empty())
    {
        return false;
    }

    FEntityChunk& LastEntityChunk = EntityChunks.back();
    assert(LastEntityChunk.EntityCount != EntitiesPerChunk);

    // POTENTIAL TODO - if(std::is_trivially_constructible_v<T>) to test if we need to run constructors

    EntityArchetypeMap->RegisterEntity(this, EntityChunks.size() - 1, LastEntityChunk.EntityCount, EntityID);

    for (std::pair<LClass* const, FComponentInfo>& Pair : ClassToComponentInfoMap)
    {
        LClass* Class = Pair.first;
        const FComponentInfo& ComponentInfo = Pair.second;

        char* ComponentAddress = GetComponentAddress(Class, LastEntityChunk, LastEntityChunk.EntityCount);

        Class->RunConstructor(reinterpret_cast<uint8_t*>(ComponentAddress));
    }

    SetEntityIDAtIndex(LastEntityChunk, LastEntityChunk.EntityCount, EntityID);

    ++LastEntityChunk.EntityCount;

    return true;
}

void LArchetype::RemoveEntity(const LEntityID EntityID)
{
    const FEntityArchetypeMapping& EntityArchetypeMapping = EntityArchetypeMap->GetMapping(EntityID);
    assert(EntityChunks.size() > EntityArchetypeMapping.ChunkIndex);
    assert(EntityArchetypeMapping.ElementIndex < EntityChunks[EntityArchetypeMapping.ChunkIndex].EntityCount);

    FEntityChunk& EntityChunk = EntityChunks[EntityArchetypeMapping.ChunkIndex];

    const std::size_t RemovedIndex = EntityArchetypeMapping.ElementIndex;
    const int LastIndex = EntityChunk.EntityCount - 1;

    const bool bSwapEntries = RemovedIndex < LastIndex;

    for (std::pair<LClass* const, FComponentInfo>& Pair : ClassToComponentInfoMap)
    {
        LClass* Class = Pair.first;
        const FComponentInfo& ComponentInfo = Pair.second;

        char* LastComponentAddress = GetComponentAddress(Class, EntityChunk, LastIndex);
        char* ComponentAddress = GetComponentAddress(Class, EntityChunk, RemovedIndex);
       
        Class->RunDestructor(reinterpret_cast<uint8_t*>(ComponentAddress));

        if (bSwapEntries)
        {
            std::memcpy(ComponentAddress, LastComponentAddress, ComponentInfo.Size);
            std::memset(LastComponentAddress, 0, ComponentInfo.Size);
        }
        else
        {
            std::memset(ComponentAddress, 0, ComponentInfo.Size);
        }
    }

    // TODO: Maybe it would be better to have FEntityChunk as a fleshed out class so we can manage the count internally?
    --EntityChunk.EntityCount;

    if (bSwapEntries)
    {
        const LEntityID LastEntityID = GetEntityIDAtIndex(EntityChunk, LastIndex);

        SetEntityIDAtIndex(EntityChunk, RemovedIndex, LastEntityID);
        SetEntityIDAtIndex(EntityChunk, LastIndex, 0);

        FEntityArchetypeMapping& MovedEntityMapping = EntityArchetypeMap->GetMapping(LastEntityID);

        MovedEntityMapping.ElementIndex = RemovedIndex;

        EntityArchetypeMap->UnregisterEntity(EntityID);
    }
    else
    {
        SetEntityIDAtIndex(EntityChunk, RemovedIndex, 0);

        EntityArchetypeMap->UnregisterEntity(EntityID);

        if (!EntityChunk.EntityCount)
        {
            _aligned_free(EntityChunk.Address);
            std::memset(EntityChunk.Address, 0, StrictChunkSize);
        }
    }
}

void LArchetype::ForEachEntityChunk(std::function<void(FEntityChunkHandle& EntityChunkHandle)> Func)
{
    // TODO: Set some flag which will prevent any entity/ component removal. And/ Or setup some functionality to allow caching what to do for later.

    for (FEntityChunk& EntityChunk : EntityChunks)
    {
        FEntityChunkHandle EntityChunkHandle(EntityChunk.Address, ClassToComponentInfoMap, EntityChunk.EntityCount, EntityIDArrayOffset);
        Func(EntityChunkHandle);
    }
}

char* LArchetype::GetComponent(const LEntityID EntityID, const std::size_t& ChunkIndex, const std::size_t& EntryIndex, LClass* ComponentClass)
{
    assert(ComponentClass);
    assert(ClassToComponentInfoMap.contains(ComponentClass));
    assert(ChunkIndex >= 0 && ChunkIndex < EntityChunks.size());
    assert(EntryIndex >= 0 && EntryIndex < EntityChunks[ChunkIndex].EntityCount);

    return GetComponentAddress(ComponentClass, EntityChunks[ChunkIndex], EntryIndex);
}

void LArchetype::DeserializeEntityData(const nlohmann::json& Json)
{
    // TEMP while I can't be bothered to refactor this properly
    LEntityID EntityID;
    AddEntity(EntityID);

    FEntityChunk& Chunk = EntityChunks.back();
    const int AddedIndex = Chunk.EntityCount - 1;

    for (const auto& Pair : ClassToComponentInfoMap)
    {
        const LClass* Class = Pair.first;
        const FComponentInfo& ComponentInfo = Pair.second;

        uint8_t* ComponentAddress = static_cast<uint8_t*>(Chunk.Address) + ComponentInfo.Offset + ComponentInfo.Size * AddedIndex;
        Class->DeserializeAddress(ComponentAddress, Json[Class->GetTypeName()]);
    }
}

void LArchetype::SerializeEntityData(nlohmann::json& Json, const std::unordered_set<LEntityID>& EntityIDs) const
{
    for (LClass* Class : Signature.Classes)
    {
        if (!Class)
        {
            Log(LLogLevel::ERROR, "LArchetype::SerializeEntityData - Invalid class in signature. Unable to serialize");
            return;
        }

        Json[Class->GetTypeName()] = {};
    }

    for (const LEntityID EntityID : EntityIDs)
    {
        const FEntityArchetypeMapping& EntityArchetypeMapping = EntityArchetypeMap->GetMapping(EntityID);

        const FEntityChunk& EntityChunk = EntityChunks[EntityArchetypeMapping.ChunkIndex];

        for (const auto& Pair : ClassToComponentInfoMap)
        {
            const LClass* Class = Pair.first;
            const FComponentInfo& ComponentInfo = Pair.second;

            uint8_t* ComponentAddress = static_cast<uint8_t*>(EntityChunk.Address) + ComponentInfo.Offset + ComponentInfo.Size * EntityArchetypeMapping.ElementIndex;

            nlohmann::json ComponentJson;
            Class->SerializeAddress(ComponentAddress, ComponentJson);

            // TEMP
            if (ComponentJson.contains("Type"))
            {
                ComponentJson.erase("Type");
            }
            // ~TEMP

            Json[Class->GetTypeName()].emplace_back(ComponentJson);
        }
    }

    Json["EntityCount"] = EntityIDs.size();
}

bool LArchetype::operator==(const LArchetype* Other) const
{
    assert(Other);
    return Signature == Other->Signature;
}

bool LArchetype::operator<(const LArchetype* Other) const
{
    //return Signature < Other->Signature;
    return true;
}

char* LArchetype::GetComponentAddress(LClass* Class, const FEntityChunk& EntityChunk, const std::size_t Index) const
{
    const FComponentInfo& ComponentInfo = ClassToComponentInfoMap.at(Class);
    return static_cast<char*>(EntityChunk.Address) + ComponentInfo.Offset + ComponentInfo.Size * Index;
}

void LArchetype::SetEntityIDAtIndex(const FEntityChunk& EntityChunk, const std::size_t Index, const LEntityID EntityID)
{
    char* EntityIDAddress = static_cast<char*>(EntityChunk.Address) + EntityIDArrayOffset + sizeof(LEntityID) * Index;
    *reinterpret_cast<LEntityID*>(EntityIDAddress) = EntityID;
}

LEntityID LArchetype::GetEntityIDAtIndex(const FEntityChunk& EntityChunk, const std::size_t Index) const
{
    char* EntityIDAddress = static_cast<char*>(EntityChunk.Address) + EntityIDArrayOffset + sizeof(LEntityID) * Index;
    return *reinterpret_cast<LEntityID*>(EntityIDAddress);
}

int LArchetype::GetNewChunksNeeded(const int EntityCount) const
{
    int FreeSlots = 0;
    for (const FEntityChunk& EntityChunk : EntityChunks)
    {
        FreeSlots += EntitiesPerChunk - EntityChunk.EntityCount;
    }

    float ChunksNeeded = static_cast<float>(EntityCount - FreeSlots) / static_cast<float>(EntitiesPerChunk);

    return ChunksNeeded > 0.0f ? static_cast<int>(ChunksNeeded + 1) : 0;
}

void LArchetype::Initialize(const FArchetypeSignature& InSignature)
{
    // TODO: Add error checking for if signature + LEntity ID types overflow CHUNK_SIZE. However unlikely
    
    Signature = InSignature;

    std::size_t TypeSizeSum = sizeof(LEntityID);
    for (LClass* Type : Signature.Classes)
    {
        assert(Type->GetByteSize() >= sizeof(LEntityID));
        TypeSizeSum += Type->GetByteSize();
    }

    std::size_t CurrentOffset = 0;

    EntitiesPerChunk = static_cast<int>(CHUNK_SIZE / TypeSizeSum);
    assert(EntitiesPerChunk > 0);

    for (LClass* Type : Signature.Classes)
    {
        MaxAlignment = std::max(MaxAlignment, Type->GetByteAlignment());

        // Should always be true since FArchetypeSignature sorts LClass' by byte size, so later entries shouldn't need alignment padding.
        assert(CurrentOffset == AlignUp(CurrentOffset, Type->GetByteAlignment()));

        ClassToComponentInfoMap[Type].Alignment = Type->GetByteAlignment();
        ClassToComponentInfoMap[Type].Offset = CurrentOffset;
        ClassToComponentInfoMap[Type].Size = Type->GetByteSize();

        CurrentOffset += Type->GetByteSize() * EntitiesPerChunk;
    }

    EntityIDArrayOffset = CurrentOffset;

    StrictChunkSize = CurrentOffset + sizeof(LEntityID) * EntitiesPerChunk;

    EntityArchetypeMap = CSystem.GetSubsystems().GetSubsystem<LEntityArchetypeMap>(true);
}

std::span<const LEntityID> FEntityChunkHandle::GetEntityIDs()
{
    char* ComponentArrayAddress = static_cast<char*>(ChunkAddress) + EntityIDArrayOffset;
    return std::span<const LEntityID>(reinterpret_cast<const LEntityID*>(ComponentArrayAddress), EntityCount);
}
