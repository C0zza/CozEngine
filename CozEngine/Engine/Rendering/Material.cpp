#include "Material.h"

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

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
