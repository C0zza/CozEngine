#include "stdafx.h"

#include "EditorSceneWindow.h"

#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Renderer.h"

LEditorSceneWindow::LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, LFrameBuffer* iEntityFrameBuffer, const char* iWindowName)
	: LEditorWindow(iWindowName), SceneFrameBuffer{ iSceneFrameBuffer }, EntityFrameBuffer{iEntityFrameBuffer}
{
	assert(SceneFrameBuffer);

	InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>();
	assert(InputManager);

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_T, GLFW_PRESS), &LEditorSceneWindow::OnToggleFocus, ToggleFocusEventHandle);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS), &LEditorSceneWindow::OnRightMouseClicked, RightMouseClickedEventHandle);

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE), &LEditorSceneWindow::OnMouseClicked, MouseClickedEventHandle);

	CSystem.GetSubsystems().AddSubsystem<LSelectedEntitySubsystem>();
}

void LEditorSceneWindow::DrawWindow()
{
	MousePos = ImGui::GetMousePos();
	TopLeft = ImGui::GetCursorScreenPos();

	ImVec2 WindowSize = ImGui::GetWindowSize();
	WindowSize.y -= ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;

	BottomRight = TopLeft + WindowSize;

	OnFocusUpdate(ImGui::IsWindowFocused());

	const ImVec2 ContentRegionAvail = ImGui::GetContentRegionAvail();

	if (SceneFrameBuffer->GetWidth() != ContentRegionAvail.x || SceneFrameBuffer->GetHeight() != ContentRegionAvail.y)
	{
		const int X = (int)ContentRegionAvail.x;
		const int Y = (int)ContentRegionAvail.y;

		glViewport(0, 0, X, Y);
		SceneFrameBuffer->RescaleBuffer(X, Y);
		EntityFrameBuffer->RescaleBuffer(X, Y);

		LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
		if (Renderer)
		{
			Renderer->SetProjectionMatrix(X, Y);
		}
	}

	ImGui::Image(
		(ImTextureID)SceneFrameBuffer->GetTexture(),
		ContentRegionAvail,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
}

void LEditorSceneWindow::PushWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
}

void LEditorSceneWindow::PopWindowStyle()
{
	ImGui::PopStyleVar(2);
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

void LEditorSceneWindow::OnRightMouseClicked()
{
	if (IsWindowHovered())
	{
		OnFocusUpdate(true);
	}
}

void LEditorSceneWindow::OnMouseClicked()
{
	if (!IsWindowHovered() || !EntityFrameBuffer)
	{
		return;
	}

	LSelectedEntitySubsystem* SelectedEntitySubsystem = CSystem.GetSubsystems().GetSubsystem<LSelectedEntitySubsystem>();
	if (!SelectedEntitySubsystem)
	{
		Log(LLogLevel::ERROR, "LEditorSceneWindow::OnMouseClicked - Invalid SelectedEntitySubsystem.");
		return;
	}

	const float XPos = MousePos.x - TopLeft.x;
	const float YPos = BottomRight.y - TopLeft.y - (MousePos.y - TopLeft.y);

	EntityFrameBuffer->Bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	float Pixel[4];
	glReadPixels(XPos, YPos, 1, 1, GL_RGBA, GL_FLOAT, &Pixel);

	EntityFrameBuffer->Unbind();

	LEntityID EntityID = static_cast<LEntityID>(Pixel[0] * 255) << 24 |
						static_cast<LEntityID>(Pixel[1] * 255) << 16 |
						static_cast<LEntityID>(Pixel[2] * 255) << 8 |
						static_cast<LEntityID>(Pixel[3] * 255);

	SelectedEntitySubsystem->SetSelectedEntityID(EntityID);
}
