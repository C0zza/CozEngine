#pragma once

#include <string>
#include <unordered_map>

#include "Misc/Logging.h"

template<typename TObjType>
class LObjRegister
{
public:
	static void RegisterObj(const std::string& TypeName, TObjType Obj);
	static TObjType Get(const std::string& TypeName);

private:
	static std::unordered_map<std::string, TObjType>& GetObjs();
};

template<typename TObjType>
void LObjRegister<TObjType>::RegisterObj(const std::string& TypeName, TObjType Obj)
{
	GetObjs()[TypeName] = Obj;
}

template<typename TObjType>
TObjType LObjRegister<TObjType>::Get(const std::string& TypeName)
{
	if (!GetObjs().contains(TypeName))
	{
		Log(LLogLevel::ERROR, "LObjRegister<TObjType>::Get - " + TypeName + " not found in register.");
	}

	return GetObjs()[TypeName];
}

template<typename TObjType>
std::unordered_map<std::string, TObjType>& LObjRegister<TObjType>::GetObjs()
{
	static std::unordered_map<std::string, TObjType> ObjMap;
	return ObjMap;
}

