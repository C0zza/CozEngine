#pragma once

#include "Subsystem.h"

#include "ResourceManagement/ResourceManager.h"
#include "Rendering/Shader.h"

class LFrameBuffer;

class LFrameBufferSubsystem : public LSubsystem
{
public:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	void DrawFrameBuffer(const LFrameBuffer* FrameBuffer);
	unsigned int GetQuadVAO() { return quadVAO; }
	LShader* GetScreenTextureShader() { return ScreenTextureShader.Get(); }

private:
	unsigned int quadVAO;
	unsigned int quadVBO;
	LResourceHandle<LShader> ScreenTextureShader;
};

