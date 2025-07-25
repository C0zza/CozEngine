#pragma once

#include "Editor/EditorWindow.h"
#include "Input/InputManager.h"

class LClass;
class LEntity;
class LFrameBuffer;
class LResourceManager;
class LSelectedEntitySubsystem;

class LEditorSceneWindow : public LEditorWindow
{
public:
	LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, LFrameBuffer* iEntityFrameBuffer, const char* iWindowName);

	virtual void PushWindowStyle() override;
	virtual void PopWindowStyle() override;

protected:
	virtual void DrawWindow() override;

private:
	// TODO: LEditor currently has ownership of these buffers for easy access from LSystem. This makes more sense to have ownership though
	LFrameBuffer* SceneFrameBuffer = nullptr;
	LFrameBuffer* EntityFrameBuffer = nullptr;

	LInputManager* InputManager = nullptr;
	LResourceManager* ResourceManager = nullptr;
	LSelectedEntitySubsystem* SelectedEntitySubsystem = nullptr;

	LInputEventHandle ToggleFocusEventHandle;
	LInputEventHandle MouseClickedEventHandle;
	LInputEventHandle RightMouseClickedEventHandle;

	void OnToggleFocus();
	void OnFocusUpdate(const bool bIsFocused);

	void OnRightMouseClicked();
	void OnMouseClicked();

	LClass* GetClassFromPath(const std::filesystem::path& Path);

	glm::vec2 LocalMouseScreenCoords;
	float WindowWidth;
	float WindowHeight;

	LEntityID HeldEntity = 0;
};

