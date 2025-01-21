#pragma once

#include <cassert>
#include <unordered_map>
#include <queue>
#include <string>

#include "ECS/ECSDefinitions.h"
#include "Logging.h"
#include "Subsystem.h"

class LTypeIdGenerator : public LSubsystem
{
public:
	template<typename T>
	LIDType GetTypeID()
	{
		const std::string TypeID = typeid(T).name();

		if (!TypeIDs.contains(TypeID))
		{
			// TODO: Are we considering 0 a valid id?
			TypeIDs.insert({ TypeID, (LIDType)(TypeIDs.size() + 1) });
		}

		return TypeIDs[TypeID];
	}

private:
	std::unordered_map<std::string, LIDType> TypeIDs;
};

class LTypeInstanceIdGenerator : public LSubsystem
{
public:
	template<typename T>
	LIDType GetNewID()
	{
		const std::string TypeID = typeid(T).name();

		if (!TypeData.contains(TypeID))
		{
			TypeData.emplace(TypeID, std::pair<int, std::queue<LIDType>>());
		}
		else if (!TypeData[TypeID].second.empty())
		{
			LIDType StaleID = TypeData[TypeID].second.front();
			TypeData[TypeID].second.pop();
			return StaleID;
		}

		return TypeData[TypeID].first++;
	}

	template<typename T>
	void UnregisterID(const LIDType ID)
	{
		const std::string TypeID = typeid(T).name();
		if (!TypeData.contains(TypeID))
		{
			Log(LLogLevel::ERROR, "LTypeInstanceIdGenerator::UnregisterID - TypeID " + TypeID + " hs not been registered yet.");
			return;
		}

		assert(TypeData[TypeID].first > ID);
		TypeData[TypeID].second.push(ID);
	}

private:
	std::map<std::string, std::pair<int, std::queue<LIDType>>> TypeData;
};