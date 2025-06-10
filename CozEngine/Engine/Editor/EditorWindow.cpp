#include "stdafx.h"
#include "EditorWindow.h"

void LEditorWindow::Draw()
{
	bIsWindowHovered = ImGui::IsWindowHovered();
	DrawWindow();
}

ImVec2 LEditorWindow::GetCurrentWindowMouseCoords() const
{
	return ImGui::GetMousePos() - ImGui::GetCursorScreenPos();
}
