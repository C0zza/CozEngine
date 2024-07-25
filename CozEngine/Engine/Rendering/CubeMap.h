#pragma once

#include "Rendering/CubeMapTexture.h"
#include "ResourceManagement/ResourceHandle.h"

class LModel;
class LShader;

class LCubeMap
{
public:
	LCubeMap(const char* i_CubeMapTexture, const char* i_CubeModel);

	void Draw();

	LResourceHandle<LCubeMapTexture> CubeMapTexture;

private:
	LResourceHandle<LShader> CubeMapShader;
	LResourceHandle<LModel> CubeModel;
};

