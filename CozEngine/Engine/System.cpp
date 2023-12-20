#include "System.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Game/Components/TestECSComponent.h"
#include "Game/Components/Movement.h"
#include "Game/DirectionalLightEntity.h"
#include "Game/PlayerEntity.h"
#include "Game/SpotLightEntity.h"
#include "Game/TestEntity.h"

#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

void System::Init()
{
	m_Renderer.Init();
}

void System::Shutdown()
{
	m_Renderer.Shutdown();
}

void System::SetupGame()
{
	LTexture::SetFlipVerticallyOnLoad(true);

	std::shared_ptr<LModel> Model = std::make_shared<LModel>("Game/Assets/backpack/backpack.obj");
	std::shared_ptr<LShader> DefaultShader = std::make_shared<LShader>("Engine/Rendering/DefaultShaders/shader.vs", "Engine/Rendering/DefaultShaders/shader.fs");

	std::shared_ptr<LModel> CubeModel = std::make_shared<LModel>();

	std::shared_ptr<LMaterial> CubeMaterial = std::make_shared<LMaterial>(DefaultShader);
	std::shared_ptr<LTexture> BoxTexture = std::make_shared<LTexture>("container.jpg", false, ETextureType::Diffuse);
	CubeMaterial->Diffuse = BoxTexture;

	std::shared_ptr<LMaterial> DefaultMaterial = std::make_shared<LMaterial>(DefaultShader);
	DefaultMaterial->Diffuse = std::make_shared<LTexture>("backpack/diffuse.jpg", false, ETextureType::Diffuse);
	DefaultMaterial->Specular = std::make_shared<LTexture>("backpack/specular.jpg", false, ETextureType::Specular);
	DefaultMaterial->SpecularShininess = 32.f;

	InputManager.Init(m_Renderer.GetWindow().get());

	// TODO: Must be a better way than registering everything here. Maybe check if it needs adding when the corresponding component is added?
	ECS.AddComponentSystem<CTestECSComponentSystem, CTestECSComponent>();
	ECS.AddComponentSystem<CModelComponentSystem, CModelComponent>();
	ECS.AddComponentSystem<LComponentSystem<CTransformComponent>, CTransformComponent>();
	ECS.AddComponentSystem<LComponentSystem<CCameraComponent>, CCameraComponent>();
	ECS.AddComponentSystem<CMovementSystem, CMovement>();
	ECS.AddComponentSystem<LComponentSystem<CSpotLightComponent>, CSpotLightComponent>();
	ECS.AddComponentSystem<LComponentSystem<CPointLightComponent>, CPointLightComponent>();
	ECS.AddComponentSystem<LComponentSystem<CDirectionalLightComponent>, CDirectionalLightComponent>();

	// TODO: No system cleaning entities up on shutdown
	LEntity* TestEntity = new CTestEntity(Model, DefaultMaterial);
	LEntity* CameraEntity = new CPlayerEntity();
	LEntity* SpotLightEntity = new CSpotLightEntity(CubeModel, CubeMaterial);
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
