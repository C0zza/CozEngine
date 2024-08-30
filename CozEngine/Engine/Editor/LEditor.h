#pragma once

#include <memory>

#include "Rendering/FrameBuffer.h"
#include "Subsystem.h"

class LInputManager;
class LRenderer;

class LEditor : public LSubsystem
{
public:
	virtual void Initialize() override;

	void Draw();

	LFrameBuffer* GetSceneFrameBuffer() { return SceneFrameBuffer.get(); }

private:
	LInputManager* InputManager = nullptr;
	LRenderer* Renderer = nullptr;

	std::unique_ptr<LFrameBuffer> SceneFrameBuffer;
	int SceneFrameBufferWidth;
	int SceneFrameBufferHeight;
};

