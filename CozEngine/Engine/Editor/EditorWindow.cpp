#include "stdafx.h"
#include "EditorWindow.h"

void LEditorWindow::Draw()
{
	bIsWindowHovered = ImGui::IsWindowHovered();
	DrawWindow();
}
