#pragma once

#include <memory>
#include <vector>

class LTexture;
class LShader;

struct LMaterial
{
	std::vector<std::shared_ptr<LTexture>> Textures;
	std::shared_ptr<LShader> Shader;
};

