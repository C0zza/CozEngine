#include "stdafx.h"

#include "EntityFactory.h"
#include "Misc/Logging.h"

std::unordered_map<std::string, std::function<std::unique_ptr<LEntity>()>> LEntityFactory::CreateFuncs = {};

void LEntityFactory::RegisterEntity(const std::string& ClassName, std::function<std::unique_ptr<LEntity>()> CreateFunc)
{
	CreateFuncs[ClassName] = CreateFunc;
}

std::unique_ptr<LEntity> LEntityFactory::CreateEntity(const std::string& ClassName)
{
	if (!CreateFuncs.contains(ClassName))
	{
		Log(LLogLevel::ERROR, "LEntityFactory::CreateEntity - Cannot create entity of " + ClassName + " as it has not been registered.");
		return nullptr;
	}

	return CreateFuncs[ClassName]();
}
