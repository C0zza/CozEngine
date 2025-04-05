#include "stdafx.h"

#include "LEditor.h"

#include "Development/LImGuiSubsystem.h"
#include "Editor/EditorWindows/EditorSceneWindow.h"
#include "Editor/DebugFrameBufferSubsystem.h"
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

	EntityFrameBuffer = std::make_unique<LFrameBuffer>(1280, 720);

	std::unique_ptr<LEditorWindow> EditorSceneWindow = std::make_unique<LEditorSceneWindow>(SceneFrameBuffer.get(), EntityFrameBuffer.get(), "Scene");
	EditorWindows.emplace_back(std::move(EditorSceneWindow));

	CSystem.GetSubsystems().AddSubsystem<LDebugFrameBufferSubsystem>();
}

void LEditor::Draw()
{
	ImGui::DockSpaceOverViewport();
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
	}

	for (std::unique_ptr<LEditorWindow>& EditorWindow : EditorWindows)
	{
		assert(EditorWindow.get());
		ImGui::Begin(EditorWindow->GetWindowName());
		{
			EditorWindow->Draw();
			ImGui::End();
		}
	}

	ImGui::Begin("Test1");
	{
		ImGui::End();
	}

	ImGui::Begin("Test2");
	{
		ImGui::End();
	}
}
