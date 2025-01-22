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

	void Use();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

	// Temp
	static void SetGlobalBool(const std::string& name, const bool b);
	static void SetGlobalInt(const std::string& name, const int i);
	static void SetGlobalFloat(const std::string& name, const float f);
	static void SetGlobalMat4(const std::string& name, const glm::mat4& mat);
	static void SetGlobalVec(const std::string& name, const glm::vec3& vec);

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LShader, VertexShaderPath, FragmentShaderPath, bUsesMatricesUBO)

private:
	unsigned int ID;

	std::string VertexShaderPath;
	std::string FragmentShaderPath;

	// TODO: Should rather have a list of UBOs this shader should register with. This'll require some UBO manager system.
	bool bUsesMatricesUBO = false;

	static std::vector<LShader*> Shaders;
};

inline void to_json(nlohmann::json& J, const LResourceHandle<LShader>& ResourceHandle)
{
	LResourceHandleHelper::ToJsonHelper(J, ResourceHandle);
}

inline void from_json(const nlohmann::json& J, LResourceHandle<LShader>& ResourceHandle)
{
	LResourceHandleHelper::FromJsonHelper(J, ResourceHandle);
}
