#pragma once

#include "ResourceManagement/ResourceHandle.h"
// Required for to_json, from_json LTexture/LShader definitions
#include "Texture.h"
#include "Shader.h"

class LMaterial : public LResource
{
public:
	virtual void Load() override;
	virtual void Unload() override {}

	void Use() const;
	const LShader* GetShader() const { return Shader.Get(); }

	LResourceHandle<LTexture> Diffuse;
	LResourceHandle<LTexture> Specular;
	LResourceHandle<LTexture> NormalMap;

	float SpecularShininess = 1.f;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LMaterial, Diffuse, Specular, SpecularShininess, NormalMap, Shader)

private:
	LResourceHandle<LShader> Shader;
};

