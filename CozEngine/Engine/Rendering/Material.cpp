#include "Material.h"

#include <glad/glad.h>

#include "Misc/Logging.h"
#include "Shader.h"
#include "Texture.h"

const LShader* LMaterial::Use()
{	
	LDrawModeSubsystem* DrawModeSubsystem = CSystem.GetSubsystems().GetSubsystem<LDrawModeSubsystem>();
	if (!DrawModeSubsystem)
	{
		Log(LLogLevel::ERROR, "LMaterial::GetActiveShader - Failed to get DrawModeSubsystem.");
		return nullptr;
	}

	const EDrawMode ActiveDrawMode = DrawModeSubsystem->GetActiveDrawMode();

	LShader* ActiveShader = nullptr;
	if (Shaders.contains(ActiveDrawMode))
	{
		ActiveShader = Shaders.at(ActiveDrawMode).Get();
		ActiveShader->Use();
		BindResources(ActiveDrawMode);
	}

	return ActiveShader;
}

bool LMaterial::HasShaderForDrawMode(const EDrawMode DrawMode) const
{
	return Shaders.contains(DrawMode) && Shaders.at(DrawMode).Get();
}
