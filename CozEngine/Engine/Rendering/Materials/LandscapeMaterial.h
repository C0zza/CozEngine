#pragma once

#include "Reflection/Reflection.h"
#include "Rendering/Material.h"
#include "Rendering/Texture.h"
#include "ResourceManagement/JsonHelper.h"

REFL_CLASS()
class LLandscapeMaterial : public LMaterial
{
	REFL_GENERATED_BODY(LLandscapeMaterial)
public:
	virtual void Load() final;

	REFL_PROP(Visible)
	LResourceHandle<LTexture> HeightMap;
	REFL_PROP(Visible)
	LResourceHandle<LTexture> GroundTexture;
	REFL_PROP(Visible)
	LResourceHandle<LTexture> WallTexture;

#if defined(COZ_EDITOR)
	LEntityID EntityID = 0;
#endif

protected:
	virtual void BindResources(const EDrawMode ActiveDrawMode);
};

