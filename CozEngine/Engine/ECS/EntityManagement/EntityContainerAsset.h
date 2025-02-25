#pragma once

#include "ECS/EntityManagement/EntityContainer.h"
#include "json.hpp"
#include "ResourceManagement/Resource.h"

class LEntityContainerAsset : public LResource
{
public:
	virtual void Load() final {}
	virtual void Unload() final {}

	const nlohmann::json& GetLevelAssetData() const { return EntitiesData; }

#if defined(COZ_EDITOR)
	void UpdateLevelAssetData(const LEntityContainer& EntityContainer);
#endif

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LEntityContainerAsset, EntitiesData)
private:
	nlohmann::json EntitiesData;
};

