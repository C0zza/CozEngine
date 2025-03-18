#include "stdafx.h"

#include "EditorSceneWindow.h"

#include "Globes.h"
#include "Rendering/FrameBuffer.h"

LEditorSceneWindow::LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, const char* iWindowName)
	: LEditorWindow(iWindowName), SceneFrameBuffer{ iSceneFrameBuffer }
{
	assert(SceneFrameBuffer);

	InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>();
	assert(InputManager);

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_T, GLFW_PRESS), &LEditorSceneWindow::OnToggleFocus, ToggleFocusEventHandle);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE), &LEditorSceneWindow::OnMouseClicked, MouseClickedEventHandle);
}

void LEditorSceneWindow::Draw()
{
	bWindowIsHovered = ImGui::IsWindowHovered();

	OnFocusUpdate(ImGui::IsWindowFocused());

	const ImVec2 ContentRegionAvail = ImGui::GetContentRegionAvail();
	if (SceneFrameBuffer->GetWidth() != ContentRegionAvail.x || SceneFrameBuffer->GetHeight() != ContentRegionAvail.y)
	{
		glViewport(0, 0, (int)ContentRegionAvail.x, (int)ContentRegionAvail.y);
		SceneFrameBuffer->RescaleBuffer((int)ContentRegionAvail.x, (int)ContentRegionAvail.y);
	}

	ImGui::Image(
		(ImTextureID)SceneFrameBuffer->GetTexture(),
		ContentRegionAvail,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
}

void LEditorSceneWindow::OnToggleFocus()
{
#if defined(COZ_EDITOR)
	if (InputManager->bInputEnabled)
	{
		OnFocusUpdate(false);
	}
	else
	{
		OnFocusUpdate(true);
	}
#endif
}

void LEditorSceneWindow::OnFocusUpdate(const bool bIsFocused)
{
#if defined(COZ_EDITOR)
	if (bIsFocused != InputManager->bInputEnabled)
	{
		const char* WindowName = bIsFocused ? GetWindowName() : NULL;
		ImGui::SetWindowFocus(WindowName);
		InputManager->ResetMousePositionData();
		InputManager->bInputEnabled = bIsFocused;
	}
#endif
}

void LEditorSceneWindow::OnMouseClicked()
{
	if (bWindowIsHovered)
	{
		Log(LLogLevel::INFO, "Scene window click");
	}
}
