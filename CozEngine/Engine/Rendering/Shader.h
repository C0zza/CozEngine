#pragma once

#include <glm/fwd.hpp>
#include <string>
#include <vector>

#include "ResourceManagement/ResourceHandleHelper.h"

class LShader : public LResource
{
public:
	LShader() : ID{ 0 } {}

	virtual void Load() override;
	virtual void Unload() override;

	unsigned int GetID() const { return ID; }

	bool HasRelevantShader() const;

	void Use();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

	// Manually defined to allow for editor functionality
	friend void to_json(nlohmann::json& J, const LShader& Shader);
	friend void from_json(const nlohmann::json& J, LShader& Shader);

private:
	inline unsigned int GetActiveShaderID() const;

	unsigned int ID;

	std::string VertexShaderPath;
	std::string FragmentShaderPath;

	// TODO: Should rather have a list of UBOs this shader should register with. This'll require some UBO manager system.
	bool bUsesMatricesUBO = false;
	bool bUsesLightingUBO = false;

#if defined(COZ_EDITOR)
	std::map<std::string, LResourceHandle<LShader>> DebugShaders;
#endif
};
