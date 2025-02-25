#include "stdafx.h"

#include <utility>

#include "EntityContainer.h"
#include "EntityRegistration.h"
#include "Misc/Logging.h"

LEntityContainer::LEntityContainer(const nlohmann::json& EntitiesData, bool& bSuccessfullyLoaded)
{
	LECS* ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	if (!ECS)
	{
		Log(LLogLevel::ERROR, "LEntityContainer::LEntityContainer - Failed to get LECS subsystem.");
		bSuccessfullyLoaded = false;
		return;
	}

	for (const auto& EntityData : EntitiesData)
	{
		if (!EntityData.contains("TypeName"))
		{
			Log(LLogLevel::ERROR, "LEntityContainer::LEntityContainer - Missing TypeName from serialized entity");
			continue;
		}

		const std::string& TypeName = EntityData["TypeName"];

		std::unique_ptr<LEntity> Entity = LEntityRegister::CallFunc(TypeName);

		const LEntityID EntityID = Entity->GetID();
		Entities.emplace(Entity->GetID(), std::move(Entity));

		LEntity* WeakEntity = Entities[EntityID].get();

		if (!EntityData.contains("ComponentData"))
		{
			continue;
		}

		for (const auto& [key, value] : EntityData["ComponentData"].items())
		{
			ECS->InitEntityComponentData(EntityID, key, value);
		}
	}

	bSuccessfullyLoaded = true;
	return;
}

#if defined(COZ_EDITOR)
void LEntityContainer::AddEntity(std::unique_ptr<LEntity> Entity)
{
	if (Entities.contains(Entity->GetID()))
	{
		Log(LLogLevel::WARNING, "LEntityContainer::AddEntity - EntityContainer already contains this entity ID: " + Entity->GetID());
		return;
	}

	Entities.emplace(Entity->GetID(), std::move(Entity));
}

void LEntityContainer::RemoveEntity(const LEntityID EntityID)
{
	if (Entities.contains(EntityID))
	{
		Entities.erase(EntityID);
	}
}
#endif
