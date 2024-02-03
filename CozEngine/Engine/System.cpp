#include "System.h"

#include "ECS/ECS.h"
#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Game/Components/Movement.h"
#include "Game/DirectionalLightEntity.h"
#include "Game/PlayerEntity.h"
#include "Game/SpotLightEntity.h"
#include "Game/TestEntity.h"
#include "Input/InputManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

#if defined(COZ_EDITOR)
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#endif

void LSystem::Run()
{
	LECS* ECS = Subsystems.AddSubsystem<LECS>();
	LRenderer* Renderer = Subsystems.AddSubsystem<LRenderer>();
	// TODO: Active input after everything is loaded so we don't pick up input while waiting
	LInputManager* InputManager = Subsystems.AddSubsystem<LInputManager>();

#if defined(COZ_EDITOR)
	// Imgui init must be after our glfw input callbacks have been setup otherwise imgui is overriden.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Renderer->GetWindow()->m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
#endif

	// TODO: Must be a better way than registering everything here. Maybe check if it needs adding when the corresponding component is added?
	ECS->AddComponentSystem<CModelComponentSystem, CModelComponent>();
	ECS->AddComponentSystem<LComponentSystem<CTransformComponent>, CTransformComponent>();
	ECS->AddComponentSystem<LComponentSystem<CCameraComponent>, CCameraComponent>();
	ECS->AddComponentSystem<CMovementSystem, CMovement>();
	ECS->AddComponentSystem<LComponentSystem<CSpotLightComponent>, CSpotLightComponent>();
	ECS->AddComponentSystem<LComponentSystem<CPointLightComponent>, CPointLightComponent>();
	ECS->AddComponentSystem<LComponentSystem<CDirectionalLightComponent>, CDirectionalLightComponent>();

	// TODO: No system cleaning entities up on shutdown
	LEntity* TestEntity = new CTestEntity();
	LEntity* CameraEntity = new CPlayerEntity();
	LEntity* SpotLightEntity = new CSpotLightEntity();
	LEntity* DirectionalLightEntity = new CDirectionalLightEntity();

	assert(Renderer->GetWindow());
	while (!Renderer->GetWindow()->ShouldClose())
	{
		Renderer->Update();
		ECS->RunComponentSystems();
		Renderer->Swap();
	}

#if defined(COZ_EDITOR)
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif
}
