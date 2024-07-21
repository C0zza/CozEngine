#pragma once

#include "Rendering/CubeMapTexture.h"
#include "ResourceManagement/ResourceHandle.h"

class LMesh;
class LShader;

class LCubeMap
{
public:
	LCubeMap(const char* i_CubeMapTexture);

	void Draw();

	LResourceHandle<LCubeMapTexture> CubeMapTexture;

private:
	LResourceHandle<LShader> CubeMapShader;
	std::unique_ptr<LMesh> CubeMesh;
};

