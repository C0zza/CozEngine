#pragma once

#include <functional>
#include <span>
#include <unordered_set>

#include "ECS/ECS2/ArchetypeSignature.h"
#include "ECS/ECSDefinitions.h"
#include "json.hpp"

class LArchetypeConfig;
class LClass;
class LEntityArchetypeMap;

struct FComponentInfo
{
	std::size_t Size = 0;
	std::size_t Alignment = 0;
	std::size_t Offset = 0;
};

struct FEntityChunkHandle
{
	FEntityChunkHandle(void* iChunkAddress, std::map<LClass*,
					   FComponentInfo>& iClassToComponentInfoMap,
					   const int iEntityCount,
					   const std::size_t& iEntityIDArrayOffset)
		: ChunkAddress{ iChunkAddress },
		  ClassToComponentInfoMap{ iClassToComponentInfoMap },
		  EntityCount{ iEntityCount },
		  EntityIDArrayOffset{ iEntityIDArrayOffset } 
	{}

	template<typename TComponentType>
	std::span<TComponentType> GetComponentSpan();

	std::span<const LEntityID> GetEntityIDs();

	int GetEntityCount() const { return EntityCount; }

private:
	void* ChunkAddress = nullptr;
	std::map<LClass*, FComponentInfo>& ClassToComponentInfoMap;
	const int EntityCount;
	std::size_t EntityIDArrayOffset;
};

class LArchetype
{
public:
	LArchetype(const LArchetypeConfig* Config);
	LArchetype(const FArchetypeSignature& Signature);
	~LArchetype();

	bool AddEntity(LEntityID& EntityID);
	void RemoveEntity(const LEntityID EntityID);

	void ForEachEntityChunk(std::function<void(FEntityChunkHandle& EntityChunkHandle)> Func);

	char* GetComponent(const LEntityID EntityID, const std::size_t& ChunkIndex, const std::size_t& EntryIndex, LClass* ComponentClass);

	void DeserializeEntityData(const nlohmann::json& Json);
	// void DeserializeEntityData(const nlohmann::json& Json);
	void SerializeEntityData(nlohmann::json& Json, const std::unordered_set<LEntityID>& EntityIDs) const;

	bool operator==(const LArchetype* Other) const;
	bool operator<(const LArchetype* Other) const;

	const FArchetypeSignature& GetSignature() const { return Signature; }

private:
	struct FEntityChunk
	{
		FEntityChunk(char* iAddress)
			: Address{ iAddress }, EntityCount{ 0 } {}

		void* const Address = nullptr;
		int EntityCount = 0;
	};

	char* GetComponentAddress(LClass* Class, const FEntityChunk& EntityChunk, const std::size_t Index) const;

	void SetEntityIDAtIndex(const FEntityChunk& EntityChunk, const std::size_t Index, const LEntityID EntityID);
	LEntityID GetEntityIDAtIndex(const FEntityChunk& EntityChunk, const std::size_t Index) const;

	int GetNewChunksNeeded(const int EntityCount) const;

	void Initialize(const FArchetypeSignature& Signature);

private:
	FArchetypeSignature Signature;
	std::map<LClass*, FComponentInfo> ClassToComponentInfoMap;
	std::vector<FEntityChunk> EntityChunks;

	// Entity chunks likely can't fully fill CHUNK_SIZE so we'll cache the size we actually use to prevent allocating unused memory
	std::size_t StrictChunkSize = 0;
	std::size_t EntityIDArrayOffset = 0;
	std::size_t MaxAlignment = 0;
	int EntitiesPerChunk = 0;

	std::map<LEntityID, std::pair<std::size_t, std::size_t>> EntityIDToIndexMap;

private:
	LEntityArchetypeMap* EntityArchetypeMap = nullptr;
};

template<typename TComponentType>
inline std::span<TComponentType> FEntityChunkHandle::GetComponentSpan()
{
	assert(ClassToComponentInfoMap.contains(TComponentType::StaticClass()));

	const FComponentInfo& ComponentInfo = ClassToComponentInfoMap.at(TComponentType::StaticClass());

	char* ComponentArrayAddress = static_cast<char*>(ChunkAddress) + ComponentInfo.Offset;

	return std::span<TComponentType>(reinterpret_cast<TComponentType*>(ComponentArrayAddress), EntityCount);
}
