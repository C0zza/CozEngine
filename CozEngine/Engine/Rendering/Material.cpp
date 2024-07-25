#include "Material.h"

#include <glad/glad.h>

#include "Misc/Logging.h"
#include "Shader.h"
#include "Texture.h"

void LMaterial::Load()
{
	// TODO: Add similar UE_LOG stuff so any usage of it can be defined out automatically in release
	if (SpecularShininess < 1.0f)
	{
		SpecularShininess = 1.0f;
		Log(LLogLevel::INFO, "Specular shininess for " + GetAssetPath() + " has been overriden. Min shininess is 1 otherwise no lighting will apply.");
	}

	bool bAnyError = false;
	if (!Diffuse.Get())
	{
		Log(LLogLevel::WARNING, "Invalid diffuse texture for " + GetAssetPath() + ". Whatever diffuse texture is bound before this will end up being used on draw.");
		bAnyError = true;
	}

	if (!Specular.Get())
	{
		Log(LLogLevel::WARNING, "WARNING - Invalid specular texture for " + GetAssetPath() + ". Whatever specular texture is bound before this will end up being used on draw.");
		bAnyError = true;
	}

	if (!NormalMap.Get())
	{
		Log(LLogLevel::WARNING, "Invalid normal map for " + GetAssetPath() + ".");
	}
	
	if (!Shader.Get())
	{
		Log(LLogLevel::WARNING, "Invalid shader for " + GetAssetPath() + ".");
		bAnyError = true;
	}

	if (!bAnyError)
	{
		Shader.Get()->Use();
		Shader.Get()->SetInt("Material.Diffuse", 0);
		Shader.Get()->SetInt("Material.Specular", 1);
		Shader.Get()->SetInt("Material.NormalMap", 2);
	}
}

void LMaterial::Use() const
{	
	Shader.Get()->Use();

	// Shader->SetVec3("Material.Ambient", Ambient);

	if (Diffuse.Get())
	{
		Diffuse.Get()->Use(0);
	}

	if (Specular.Get())
	{
		Specular.Get()->Use(1);
		Shader.Get()->SetFloat("Material.Shininess", SpecularShininess);
	}

	if (NormalMap.Get())
	{
		NormalMap.Get()->Use(2);
	}
}
