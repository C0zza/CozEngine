#include "Material.h"

#include <glad/glad.h>
#include <iostream>

#include "Shader.h"
#include "Texture.h"

void LMaterial::Load()
{
	if (SpecularShininess < 1.0f)
	{
		SpecularShininess = 1.0f;
		std::cout << "INFO - Specular shininess for " << GetAssetPath() << " has been overriden. Min shininess is 1 otherwise no lighting will apply." << "\n";
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
	else
	{
		std::cout << "WARNING - Invalid diffuse texture. Previously set texture will be used." << "\n";
	}

	if (Specular.Get())
	{
		Shader.Get()->SetInt("Material.Specular", 1);
		Specular.Get()->Use(1);
		Shader.Get()->SetFloat("Material.Shininess", SpecularShininess);
	}
	else
	{
		std::cout << "WARNING - Invalid specular texture. Previously set texture will be used." << "\n";
	}

	glActiveTexture(GL_TEXTURE0);
}
