#pragma once

#include "ECS/EntityManagement/EntityContainer.h"
#include "json.hpp"
#include "Reflection/Reflection.h"
#include "ResourceManagement/Resource.h"

REFL_CLASS()
class LEntityContainerAsset : public LResource
{
	REFL_GENERATED_BODY(LEntityContainerAsset)
public:
	virtual void Load() final {}
	virtual void Unload() final {}

	const nlohmann::json& GetLevelAssetData() const { return EntitiesData; }

#if defined(COZ_EDITOR)
	void UpdateLevelAssetData(const LEntityContainer& EntityContainer);
#endif

private:
	REFL_PROP()
	nlohmann::json EntitiesData;
};

