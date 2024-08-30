#include "stdafx.h"

#include "LEditor.h"

#include "Development/LImGuiSubsystem.h"
#include "Globes.h"
#include "imgui/imgui.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

void LEditor::Initialize()
{
	assert(CSystem.GetSubsystems().GetSubsystem<LImGuiSubsystem>());

	Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	assert(Renderer);

	SceneFrameBuffer = std::make_unique<LFrameBuffer>(1280, 720);
	glfwSetFramebufferSizeCallback(Renderer->GetWindow()->GetWindow(), 0);

	InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>();
	assert(InputManager);
}

void LEditor::Draw()
{
	//ImGui::ShowDemoWindow();

	ImGui::DockSpaceOverViewport();
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Scene");
	{
		InputManager->bEditorSceneFocused = ImGui::IsWindowFocused();

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

		ImGui::End();
	}
}
