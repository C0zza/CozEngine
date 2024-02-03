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

	if (!Diffuse.Get())
	{
		Log(LLogLevel::WARNING, "Invalid diffuse texture for " + GetAssetPath() + ". Whatever diffuse texture is bound before this will end up being used on draw.");
	}

	if (!Specular.Get())
	{
		Log(LLogLevel::WARNING, "WARNING - Invalid specular texture for " + GetAssetPath() + ". Whatever specular texture is bound before this will end up being used on draw.");
	}
}

void LMaterial::Use() const
{	
	// Temp save on setting shader each mesh draw
	static LShader* CurrentShader = nullptr;
	if (!CurrentShader)
	{
		assert(Shader.Get());
		CurrentShader = Shader.Get();
		CurrentShader->Use();
	}

	// Shader->SetVec3("Material.Ambient", Ambient);

	if (Diffuse.Get())
	{
		Shader.Get()->SetInt("Material.Diffuse", 0);
		Diffuse.Get()->Use(0);
	}

	if (Specular.Get())
	{
		Shader.Get()->SetInt("Material.Specular", 1);
		Specular.Get()->Use(1);
		Shader.Get()->SetFloat("Material.Shininess", SpecularShininess);
	}

	glActiveTexture(GL_TEXTURE0);
}
