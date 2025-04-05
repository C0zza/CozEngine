#pragma once

#include <memory>
#include <vector>

#include "Editor/EditorWindow.h"
#include "Rendering/FrameBuffer.h"
#include "Subsystem.h"

class LEditorWindow;
class LRenderer;

class LEditor : public LSubsystem
{
public:
	virtual void Initialize() override;

	void Draw();

	LFrameBuffer* GetSceneFrameBuffer() { return SceneFrameBuffer.get(); }
	LFrameBuffer* GetEntityFrameBuffer() { return EntityFrameBuffer.get(); }

private:
	LRenderer* Renderer = nullptr;

	std::unique_ptr<LFrameBuffer> SceneFrameBuffer;
	std::unique_ptr<LFrameBuffer> EntityFrameBuffer;
	
	std::vector<std::unique_ptr<LEditorWindow>> EditorWindows;
};

