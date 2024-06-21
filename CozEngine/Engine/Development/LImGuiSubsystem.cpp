#include "LImGuiSubsystem.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Globes.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

bool LImGuiSubsystem::HasRequiredSubsystems() const
{
	return CSystem.GetSubsystems().GetSubsystem<LRenderer>();
}

void LImGuiSubsystem::PreTick()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void LImGuiSubsystem::PostTick()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LImGuiSubsystem::Initialize()
{
	LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	LInputManager* InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>(true);

	// Imgui init must be after our glfw input callbacks have been setup otherwise imgui is overriden.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Renderer->GetWindow()->m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	bTickable = true;
}

void LImGuiSubsystem::Deinitialize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
