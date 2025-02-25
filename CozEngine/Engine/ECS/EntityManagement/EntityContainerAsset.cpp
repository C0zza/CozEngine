#include "stdafx.h"
#include "EntityContainerAsset.h"

#if defined(COZ_EDITOR)
#include "ECS/ECS.h"

void LEntityContainerAsset::UpdateLevelAssetData(const LEntityContainer& EntityContainer)
{
	EntitiesData.clear();

	const std::map<LEntityID, std::unique_ptr<LEntity>>& Entities = EntityContainer.GetEntities();

	EntitiesData = {};

	LECS* ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	if (!ECS)
	{
		Log(LLogLevel::ERROR, "LEntityContainerAsset::UpdateLevelAssetData - Failed to get ECS.");
		return;
	}

	for (const std::pair<const LEntityID, std::unique_ptr<LEntity>>& Pair : Entities)
	{
		const LEntityID ID = Pair.first;
		const std::unique_ptr<LEntity>& Entity = Pair.second;

		if (!Entity.get())
		{
			Log(LLogLevel::ERROR, "LEntityContainerAsset::UpdateLevelAssetData - Invalid entity provided. Skipping.");
			continue;
		}

		nlohmann::json EntityJson;

		EntityJson["ComponentData"] = {};
		nlohmann::json& ComponentData = EntityJson["ComponentData"];

		ECS->GetEntityComponentData(ID, ComponentData);

		EntityJson["TypeName"] = Entity->GetTypeName();

		EntitiesData.push_back(EntityJson);
	}
}
#endif
