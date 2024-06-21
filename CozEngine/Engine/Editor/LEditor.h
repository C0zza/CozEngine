#pragma once

#include <memory>

#include "Rendering/FrameBuffer.h"
#include "Subsystem.h"

class LRenderer;

class LEditor : public LSubsystem
{
public:
	virtual void Initialize() override;

	virtual void Tick() override;

	LFrameBuffer* GetSceneFrameBuffer() { return SceneFrameBuffer.get(); }

private:
	LRenderer* Renderer = nullptr;

	std::unique_ptr<LFrameBuffer> SceneFrameBuffer;
	int SceneFrameBufferWidth;
	int SceneFrameBufferHeight;
};

