#include "stdafx.h"
#include "ModelMaterial.h"

void LModelMaterial::Load()
{
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

	if (!Shaders.contains(EDrawMode::Default) || !Shaders.at(EDrawMode::Default).Get())
	{
		Log(LLogLevel::WARNING, "Invalid default shader for " + GetAssetPath() + ".");
		bAnyError = true;
	}

	if (!bAnyError)
	{
		LShader* DefaultShader = Shaders.at(EDrawMode::Default).Get();
		DefaultShader->Use();
		DefaultShader->SetInt("Material.Diffuse", 0);
		DefaultShader->SetInt("Material.Specular", 1);
		DefaultShader->SetInt("Material.NormalMap", 2);
	}
}

void LModelMaterial::BindResources(const EDrawMode ActiveDrawMode)
{
	// TODO: Can we instead register relevant functors on startup stored on LMaterial? Saves extra switch logic, duplicate code for
	// shared purpose draw mode shaders and also would prevent the need for vtable lookup.
	switch (ActiveDrawMode)
	{
	case EDrawMode::Default:
		if (Diffuse.Get())
		{
			Diffuse->Use(0);
		}

		if (Specular.Get())
		{
			Specular->Use(1);
			Shaders.at(ActiveDrawMode)->SetFloat("Material.Shininess", SpecularShininess);
		}

		if (NormalMap.Get())
		{
			NormalMap->Use(2);
		}
		break;
#if defined(COZ_EDITOR)
	case EDrawMode::EntityFrameBuffer:
		if (Shaders.contains(EDrawMode::EntityFrameBuffer))
		{
			// Assumes EntityID has been updated appropriately
			Shaders.at(EDrawMode::EntityFrameBuffer)->SetUInt("EntityID", EntityID);
		}
		break;
#endif
	default:
		break;
	}
}
