#pragma once

#include "Editor/EditorWindow.h"
#include "Input/InputManager.h"

class LFrameBuffer;

class LEditorSceneWindow : public LEditorWindow
{
public:
	LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, const char* iWindowName);
	
	virtual void Draw() override;

private:
	LFrameBuffer* SceneFrameBuffer = nullptr;
	LInputManager* InputManager = nullptr;

	LInputEventHandle ToggleFocusEventHandle;
	LInputEventHandle MouseClickedEventHandle;

	void OnToggleFocus();
	void OnFocusUpdate(const bool bIsFocused);

	void OnMouseClicked();

	bool bWindowIsHovered = false;
};

