#include "System.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Game/Components/Movement.h"
#include "Game/DirectionalLightEntity.h"
#include "Game/PlayerEntity.h"
#include "Game/SpotLightEntity.h"
#include "Game/TestEntity.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void System::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	m_Renderer.Shutdown();
}

void System::SetupGame()
{
	LTexture::SetFlipVerticallyOnLoad(true);

	m_Renderer.Init();
	InputManager.Init(m_Renderer.GetWindow().get());

	// Imgui init must be after our glfw input callbacks have been setup otherwise imgui is overriden.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Renderer.GetWindow()->m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// TODO: Must be a better way than registering everything here. Maybe check if it needs adding when the corresponding component is added?
	ECS.AddComponentSystem<CModelComponentSystem, CModelComponent>();
	ECS.AddComponentSystem<LComponentSystem<CTransformComponent>, CTransformComponent>();
	ECS.AddComponentSystem<LComponentSystem<CCameraComponent>, CCameraComponent>();
	ECS.AddComponentSystem<CMovementSystem, CMovement>();
	ECS.AddComponentSystem<LComponentSystem<CSpotLightComponent>, CSpotLightComponent>();
	ECS.AddComponentSystem<LComponentSystem<CPointLightComponent>, CPointLightComponent>();
	ECS.AddComponentSystem<LComponentSystem<CDirectionalLightComponent>, CDirectionalLightComponent>();

	// TODO: No system cleaning entities up on shutdown
	LEntity* TestEntity = new CTestEntity();
	LEntity* CameraEntity = new CPlayerEntity();
	LEntity* SpotLightEntity = new CSpotLightEntity();
	LEntity* DirectionalLightEntity = new CDirectionalLightEntity();
}

void System::Run()
{
	assert(m_Renderer.GetWindow());
	while (!m_Renderer.GetWindow()->ShouldClose())
	{
		m_Renderer.Tick();

		ECS.RunComponentSystems();

		m_Renderer.PostTick();
	}

	Shutdown();
}
