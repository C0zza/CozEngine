#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
class LShader
{
public:
	LShader(const char* VertexPath, const char* FragmentPath);
	~LShader();

	unsigned int GetID() { return ID; }

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
private:
	unsigned int ID;

	static std::vector<LShader*> Shaders;
};
