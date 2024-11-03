#pragma once

#include "Editor/EditorWindow.h"

class LFrameBuffer;
class LInputManager;

class LEditorSceneWindow : public LEditorWindow
{
public:
	LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, const char* iWindowName);

	virtual void Draw() override;

private:
	LFrameBuffer* SceneFrameBuffer = nullptr;
	LInputManager* InputManager = nullptr;
};

