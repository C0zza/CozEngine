#pragma once

#include "Reflection/Reflection.h"
#include "Rendering/Material.h"
#include "Rendering/Texture.h"
#include "ResourceManagement/JsonHelper.h"

REFL_CLASS()
class LModelMaterial : public LMaterial
{
	REFL_GENERATED_BODY(LModelMaterial)
public:
	virtual void Load() final;

	REFL_PROP(Visible)
	LResourceHandle<LTexture> Diffuse;
	REFL_PROP(Visible)
	LResourceHandle<LTexture> Specular;
	REFL_PROP(Visible)
	LResourceHandle<LTexture> NormalMap;

	REFL_PROP()
	float SpecularShininess = 1.f;

#if defined(COZ_EDITOR)
	// TODO: Could be better to have some single var to call into. Both this and LandscapeMaterial have this atm.
	LEntityID EntityID = 0;
#endif

protected:
	virtual void BindResources(const EDrawMode ActiveDrawMode);
};

