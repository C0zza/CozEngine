#pragma once

#include "Rendering/Material.h"
#include "Rendering/Texture.h"
#include "ResourceManagement/JsonHelper.h"

class LLandscapeMaterial : public LMaterial
{
public:
	virtual void Load() final;

	LResourceHandle<LTexture> HeightMap;
	LResourceHandle<LTexture> GroundTexture;
	LResourceHandle<LTexture> WallTexture;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_BASE(LLandscapeMaterial, LMaterial, HeightMap, GroundTexture, WallTexture)

protected:
	virtual void BindResources(const EDrawMode ActiveDrawMode);
};

