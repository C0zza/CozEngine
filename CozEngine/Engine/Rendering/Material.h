#pragma once

#include "Editor/DrawModeSubsystem.h"
#include "ResourceManagement/ResourceHandle.h"
#include "Shader.h"

REFL_CLASS()
class LMaterial : public LResource
{
	REFL_GENERATED_BODY(LMaterial)
public:
	const LShader* Use();

	bool HasShaderForDrawMode(const EDrawMode DrawMode) const;

protected:
	virtual void BindResources(const EDrawMode ActiveDrawMode) {}

	REFL_PROP(Visible)
	std::map<EDrawMode, LResourceHandle<LShader>> Shaders;
};

