#pragma once

#include "Rendering/Material.h"
#include "Rendering/Texture.h"
#include "ResourceManagement/JsonHelper.h"

class LModelMaterial : public LMaterial
{
public:
	virtual void Load() final;

	LResourceHandle<LTexture> Diffuse;
	LResourceHandle<LTexture> Specular;
	LResourceHandle<LTexture> NormalMap;

	float SpecularShininess = 1.f;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_BASE(LModelMaterial, LMaterial, Diffuse, Specular, NormalMap, SpecularShininess)

protected:
	virtual void BindResources(const EDrawMode ActiveDrawMode);
};

