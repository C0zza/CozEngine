#pragma once

#include "Editor/EditorWindow.h"
#include "Input/InputManager.h"

class LFrameBuffer;

class LEditorSceneWindow : public LEditorWindow
{
public:
	LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, LFrameBuffer* iEntityFrameBuffer, const char* iWindowName);
	
	virtual void Draw() override;

	virtual void PushWindowStyle() override;
	virtual void PopWindowStyle() override;

private:
	// TODO: LEditor currently has ownership of these buffers for easy access from LSystem. This makes more sense to have ownership though
	LFrameBuffer* SceneFrameBuffer = nullptr;
	LFrameBuffer* EntityFrameBuffer = nullptr;

	LInputManager* InputManager = nullptr;

	LInputEventHandle ToggleFocusEventHandle;
	LInputEventHandle MouseClickedEventHandle;

	void OnToggleFocus();
	void OnFocusUpdate(const bool bIsFocused);

	void OnMouseClicked();

	bool bWindowIsHovered = false;

	ImVec2 MousePos;
	ImVec2 TopLeft;
	ImVec2 BottomRight;
};

