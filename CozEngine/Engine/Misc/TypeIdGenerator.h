#pragma once

#include <cassert>
#include <unordered_map>
#include <queue>
#include <string>

#include "ECS/ECSDefinitions.h"

class LUniqueTypeIdGenerator
{
public:
	template<typename T>
	static LIDType GetTypeID()
	{
		std::string TypeID = typeid(T).name();

		if (!TypeIDs.contains(TypeID))
		{
			// TODO: Are we considering 0 a valid id?
			TypeIDs.insert({ TypeID, (LIDType)(TypeIDs.size() + 1) });
		}

		return TypeIDs[TypeID];
	}

private:
	static std::unordered_map<std::string, LIDType> TypeIDs;
};

template<typename T>
class LTypeIdGenerator
{
public:
	static LIDType GetNewID()
	{
		if (!StaleIDs.empty())
		{
			LIDType& StaleID = StaleIDs.front();
			StaleIDs.pop();
			return StaleID;
		}
		else
		{
			return ++IdCount;
		}
	}

	static void UnregisterID(const LIDType ID)
	{
		assert(IdCount > ID);
		StaleIDs.push(ID);
	}

private:
	static LIDType IdCount;
	static std::queue<LIDType> StaleIDs;
};

template<typename T>
LIDType LTypeIdGenerator<T>::IdCount = 0;

template<typename T>
std::queue<LIDType> LTypeIdGenerator<T>::StaleIDs = {};

// TODO: Will probably also be useful to be able to access LEntity by ID. Wait for use case.

//template<typename T>
//class LTypeManager : public LTypeIdGenerator<T>
//{
//public:
//	static LIDType RegisterNewObject()
//	{
//
//	}
//
//private:
//	static std::unordered_map<
//};