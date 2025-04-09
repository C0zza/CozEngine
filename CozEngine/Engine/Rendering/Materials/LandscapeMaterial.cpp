#include "stdafx.h"
#include "LandscapeMaterial.h"

void LLandscapeMaterial::Load()
{
	if (!HeightMap.Get())
	{
		Log(LLogLevel::WARNING, "Invalid HeightMap texture for " + GetAssetPath() + ".");
	}

	if (!GroundTexture.Get())
	{
		Log(LLogLevel::WARNING, "WARNING - Invalid GroundTexture for " + GetAssetPath() + ".");
	}

	if (!WallTexture.Get())
	{
		Log(LLogLevel::WARNING, "Invalid WallTexture for " + GetAssetPath() + ".");
	}
}

void LLandscapeMaterial::BindResources(const EDrawMode ActiveDrawMode)
{
	switch (ActiveDrawMode)
	{
	case EDrawMode::Default:
		HeightMap->Use(0);
		GroundTexture->Use(1);
		WallTexture->Use(2);
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
