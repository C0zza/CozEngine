#include "Material.h"

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

LMaterial::LMaterial(const std::shared_ptr<LShader>& i_Shader)
{
	Shader = i_Shader;
}

void LMaterial::Use() const
{	
	// Temp save on setting shader each mesh draw
	static LShader* CurrentShader = nullptr;
	if (!CurrentShader)
	{
		assert(Shader.get());
		CurrentShader = Shader.get();
		assert(CurrentShader);
		CurrentShader->Use();
	}
	else
	{
		assert(CurrentShader);
	}

	Shader->SetVec3("Material.Ambient", Ambient);

	if (Diffuse.get())
	{
		Shader->SetInt("Material.Diffuse", 0);
		Diffuse->Use(0);
	}

	if (Specular.get())
	{
		Shader->SetInt("Material.Specular", 1);
		Specular->Use(1);
		Shader->SetFloat("Material.Shininess", SpecularShininess);
	}

	glActiveTexture(GL_TEXTURE0);
}
