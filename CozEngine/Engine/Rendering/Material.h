#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

class LTexture;
class LShader;

class LMaterial
{
public:
	LMaterial(const std::shared_ptr<LShader>& i_Shader);

	void Use() const;
	const LShader* GetShader() const { return Shader.get(); }

	glm::vec3 Ambient = glm::vec3(0.f, 0.f, 0.f);
	std::shared_ptr<LTexture> Diffuse = nullptr;
	std::shared_ptr<LTexture> Specular = nullptr;
	float SpecularShininess = 0.f;

private:
	std::shared_ptr<LShader> Shader = nullptr;
};

