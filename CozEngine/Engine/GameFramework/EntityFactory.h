#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "ECS/Entity.h"

class LEntityFactory
{
public:
	static void RegisterEntity(const std::string& ClassName, std::function<std::unique_ptr<LEntity>()> CreateFunc);

	static std::unique_ptr<LEntity> CreateEntity(const std::string& ClassName);

	static std::unordered_map<std::string, std::function<std::unique_ptr<LEntity>()>>& GetCreateFuncs() { return CreateFuncs; }

private:
	static std::unordered_map<std::string, std::function<std::unique_ptr<LEntity>()>> CreateFuncs;
};

