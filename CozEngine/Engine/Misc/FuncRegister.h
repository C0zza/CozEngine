#pragma once

#include <string>
#include <unordered_map>

#include "Misc/Logging.h"

template<typename TFuncReturnType>
class LFuncRegister
{
	using TFuncType = std::function<TFuncReturnType()>;
public:
	static void RegisterFunc(const std::string& TypeName, TFuncType Func);
	static TFuncReturnType CallFunc(const std::string& TypeName);
	static std::unordered_map<std::string, TFuncType>& GetFuncs();
};

template<typename TFuncReturnType>
std::unordered_map<std::string, std::function<TFuncReturnType()>>& LFuncRegister<TFuncReturnType>::GetFuncs()
{
	static std::unordered_map<std::string, TFuncType> Funcs;
	return Funcs;
}

template<typename TFuncReturnType>
TFuncReturnType LFuncRegister<TFuncReturnType>::CallFunc(const std::string& TypeName)
{
	if (!GetFuncs().contains(TypeName))
	{
		Log(LLogLevel::ERROR, "LFuncRegister<TFuncReturnType>::CallFunc - " + TypeName + " not found in register.");
	}

	return GetFuncs()[TypeName]();
}

template<typename TFuncReturnType>
void LFuncRegister<TFuncReturnType>::RegisterFunc(const std::string& TypeName, TFuncType Func)
{
	GetFuncs()[TypeName] = Func;
}

