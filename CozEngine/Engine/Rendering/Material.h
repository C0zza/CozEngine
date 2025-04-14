#pragma once

#include "Editor/DrawModeSubsystem.h"
#include "ResourceManagement/ResourceHandle.h"
#include "Shader.h"

class LMaterial : public LResource
{
public:
	const LShader* Use();

	bool HasShaderForDrawMode(const EDrawMode DrawMode) const;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LMaterial, Shaders)

protected:
	virtual void BindResources(const EDrawMode ActiveDrawMode) {}

	std::map<EDrawMode, LResourceHandle<LShader>> Shaders;
};

