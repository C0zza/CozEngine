#include "stdafx.h"

#include "LEditor.h"

#include "Development/LImGuiSubsystem.h"
#include "Globes.h"
#include "imgui/imgui.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

void LEditor::Initialize()
{
	CSystem.GetSubsystems().AddSubsystem<LImGuiSubsystem>();
	bTickable = true;

	Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	assert(Renderer);

	SceneFrameBuffer = std::make_unique<LFrameBuffer>(800, 600);
	glfwSetFramebufferSizeCallback(Renderer->GetWindow()->GetWindow(), 0);

	Renderer->SetCustomFrameBuffer(SceneFrameBuffer.get());
}

void LEditor::Tick()
{
	//ImGui::ShowDemoWindow();

	ImGui::DockSpaceOverViewport();
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Scene");
	{
		const ImVec2 ContentRegionAvail = ImGui::GetContentRegionAvail();
		if (SceneFrameBuffer->GetWidth() != ContentRegionAvail.x || SceneFrameBuffer->GetHeight() != ContentRegionAvail.y)
		{
			glViewport(0, 0, ContentRegionAvail.x, ContentRegionAvail.y);
			SceneFrameBuffer->RescaleBuffer(ContentRegionAvail.x, ContentRegionAvail.y);
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
