#include "stdafx.h"

#include "EditorSceneWindow.h"

#include "Globes.h"
#include "Input/InputManager.h"
#include "Rendering/FrameBuffer.h"

LEditorSceneWindow::LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, const char* iWindowName)
	: LEditorWindow(iWindowName), SceneFrameBuffer{ iSceneFrameBuffer }
{
	assert(SceneFrameBuffer);

	InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>();
	assert(InputManager);
}

void LEditorSceneWindow::Draw()
{
#if defined(COZ_EDITOR)
	InputManager->bEditorSceneFocused = ImGui::IsWindowFocused();
#endif

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
