#pragma once

#include <glm/fwd.hpp>
#include <string>
#include <vector>

#include "ResourceManagement/ResourceHandleHelper.h"
#include "Reflection/Reflection.h"

REFL_CLASS()
class LShader : public LResource
{
	REFL_GENERATED_BODY(LShader)
public:
	LShader() : ID{ 0 }, VertexShaderPath{ "" }, FragmentShaderPath{ "" } {}

	virtual void Load() override;
	virtual void Unload() override;

	unsigned int GetID() const { return ID; }

	void Use();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetUInt(const std::string& name, unsigned int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
	unsigned int ID;

	REFL_PROP(Visible)
	std::string VertexShaderPath;
	REFL_PROP(Visible)
	std::string FragmentShaderPath;

	// TODO: Should rather have a list of UBOs this shader should register with. This'll require some UBO manager system.
	REFL_PROP(Visible)
	bool bUsesMatricesUBO = false;
	REFL_PROP(Visible)
	bool bUsesLightingUBO = false;
};
