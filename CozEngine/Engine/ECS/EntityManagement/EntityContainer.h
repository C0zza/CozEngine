#pragma once

#include "ECS/ECSDefinitions.h"
#include "ECS/Entity.h"

class LEntityContainer
{
public:
	LEntityContainer() = default;
	LEntityContainer(const nlohmann::json& EntitiesData, bool& bSuccessfullyLoaded);

#if defined(COZ_EDITOR)
	void AddEntity(std::unique_ptr<LEntity> Entity);
	void RemoveEntity(const LEntityID EntityID);
	const std::map<LEntityID, std::unique_ptr<LEntity>>& GetEntities() const { return Entities; }
#endif

private:
	std::map<LEntityID, std::unique_ptr<LEntity>> Entities;
};

