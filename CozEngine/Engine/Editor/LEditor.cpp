#include "stdafx.h"

#include "LEditor.h"

#include "Development/LImGuiSubsystem.h"
#include "Editor/AssetRegistry.h"
#include "Editor/EditorWindows/ContentBrowserWindow.h"
#include "Editor/EditorWindows/EditorSceneWindow.h"
#include "Editor/EditorWindows/InspectorWindow.h"
#include "Globes.h"
#include "imgui/imgui.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

void LEditor::Initialize()
{
	assert(CSystem.GetSubsystems().GetSubsystem<LImGuiSubsystem>());

	Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	assert(Renderer);

	SceneFrameBuffer = std::make_unique<LFrameBuffer>(1280, 720, GL_RGB);
	glfwSetFramebufferSizeCallback(Renderer->GetWindow()->GetWindow(), 0);

	EntityFrameBuffer = std::make_unique<LFrameBuffer>(1280, 720, GL_RGBA);

	std::unique_ptr<LEditorWindow> EditorSceneWindow = std::make_unique<LEditorSceneWindow>(SceneFrameBuffer.get(), EntityFrameBuffer.get(), "Scene");
	EditorWindows.emplace(EditorSceneWindow->GetWindowName(), std::move(EditorSceneWindow));

	std::unique_ptr<LInspectorWindow> InspectorWindow = std::make_unique<LInspectorWindow>("Inspector");
	EditorWindows.emplace(InspectorWindow->GetWindowName(), std::move(InspectorWindow));

	std::unique_ptr<LContentBrowserWindow> ContentBrowserWindow = std::make_unique<LContentBrowserWindow>("Content Browser");
	EditorWindows.emplace(ContentBrowserWindow->GetWindowName(), std::move(ContentBrowserWindow));

	CSystem.GetSubsystems().AddSubsystem<LAssetRegistry>();
}

void LEditor::Draw()
{
	ImGui::DockSpaceOverViewport();
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
	}

	ImGui::ShowDemoWindow();

	for (std::pair<const std::string, std::unique_ptr<LEditorWindow>>& Pair : EditorWindows)
	{
		LEditorWindow* EditorWindow = Pair.second.get();

		assert(EditorWindow);
		EditorWindow->PushWindowStyle();
		ImGui::Begin(EditorWindow->GetWindowName());
		{
			EditorWindow->Draw();
			ImGui::End();
		}
		EditorWindow->PopWindowStyle();
	}

	for (const std::string& Window : WindowsToUnregister)
	{
		EditorWindows.erase(Window);
	}
	WindowsToUnregister.clear();
}

void LEditor::UnregisterWindow(const std::string& WindowName)
{
	if (!EditorWindows.contains(WindowName))
	{
		Log(LLogLevel::INFO, "LEditorWindow::UnregisterWindow - Editor window: " + WindowName + " not found.");
		return;
	}

	WindowsToUnregister.insert(WindowName);
}
