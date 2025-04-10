#include "stdafx.h"

#include "EditorSceneWindow.h"

#include "Globes.h"
#include "Rendering/FrameBuffer.h"

LEditorSceneWindow::LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, LFrameBuffer* iEntityFrameBuffer, const char* iWindowName)
	: LEditorWindow(iWindowName), SceneFrameBuffer{ iSceneFrameBuffer }, EntityFrameBuffer{iEntityFrameBuffer}
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
	MousePos = ImGui::GetMousePos();
	TopLeft = ImGui::GetCursorScreenPos();
	BottomRight = ImGui::GetContentRegionMax();
	BottomRight.y += ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;

	OnFocusUpdate(ImGui::IsWindowFocused());

	const ImVec2 ContentRegionAvail = ImGui::GetContentRegionAvail();

	if (SceneFrameBuffer->GetWidth() != ContentRegionAvail.x || SceneFrameBuffer->GetHeight() != ContentRegionAvail.y)
	{
		const int X = (int)ContentRegionAvail.x;
		const int Y = (int)ContentRegionAvail.y;

		glViewport(0, 0, X, Y);
		SceneFrameBuffer->RescaleBuffer(X, Y);
		EntityFrameBuffer->RescaleBuffer(X, Y);
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

void LEditorSceneWindow::OnMouseClicked()
{
	if (bWindowIsHovered)
	{
		if (!EntityFrameBuffer)
		{
			return;
		}

		const float XSize = BottomRight.x - TopLeft.x;
		const float YSize = BottomRight.y - TopLeft.y;

		// Flip y to match texture format and adjust for window start location
		MousePos.y = YSize - MousePos.y + TopLeft.y;
		MousePos.x += TopLeft.x;

		const float XPos = MousePos.x * XSize / EntityFrameBuffer->GetWidth();
		const float YPos = MousePos.y * YSize / EntityFrameBuffer->GetHeight();

		EntityFrameBuffer->Bind();
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		float Pixel[4];
		glReadPixels(XPos, YPos, 1, 1, GL_RGBA, GL_FLOAT, &Pixel);

		EntityFrameBuffer->Unbind();

		LEntityID EntityID = static_cast<LEntityID>(Pixel[0] * 255) << 24 |
							 static_cast<LEntityID>(Pixel[1] * 255) << 16 |
							 static_cast<LEntityID>(Pixel[2] * 255) << 8 |
							 static_cast<LEntityID>(Pixel[3] * 255);

		// 0 Should represent no object has been drawn to the EntityFrameBuffer here. Dependent on the clear color of the buffer and that
		// LTypeInstanceIdGenerator IDs start from 0.
		if (EntityID != 0)
		{
			// TODO: Record this as the selected entity somewhere
		}
	}
}
