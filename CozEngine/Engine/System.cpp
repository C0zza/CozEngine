#include "System.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Object.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Components/Lighting/DirectionalLightComponent.h"
#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Rendering/Material.h"
#include "Components/ModelComponent.h"
#include "Rendering/Model.h"

#include "Game/Components/TestComponent.h"
#include "Game/Components/TestECSComponent.h"
#include "Game/Components/Movement.h"
#include "Game/PlayerEntity.h"
#include "Game/SpotLightEntity.h"
#include "Game/TestEntity.h"

System::System()
{
}

System::~System()
{
}

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

	CObject* DirectionalLightObject = new CObject();
	DirectionalLightObject->Transform.SetRotation(glm::vec3(0.f, 45.f, 0.f));
	Objects.emplace_back(DirectionalLightObject);
	CDirectionalLightComponent* DirectionalLight = DirectionalLightObject->Components.AddComponent<CDirectionalLightComponent>(DirectionalLightObject);
	DirectionalLight->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	DirectionalLight->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	DirectionalLight->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

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
	ECS.AddComponentSystem<CECSModelComponentSystem, CECSModelComponent>();
	ECS.AddComponentSystem<LComponentSystem<CECSTransformComponent>, CECSTransformComponent>();
	ECS.AddComponentSystem<LComponentSystem<CECSCameraComponent>, CECSCameraComponent>();
	ECS.AddComponentSystem<CMovementSystem, CMovement>();
	ECS.AddComponentSystem<LComponentSystem<CECSSpotLightComponent>, CECSSpotLightComponent>();

	// TODO: No system cleaning entities up on shutdown
	LEntity* TestEntity = new CTestEntity(Model, DefaultMaterial);
	LEntity* CameraEntity = new CPlayerEntity();
	LEntity* SpotLightEntity = new CSpotLightEntity(CubeModel, CubeMaterial);
}

void System::Run()
{
	assert(m_Renderer.GetWindow());
	while (!m_Renderer.GetWindow()->ShouldClose())
	{
		// PointLightTransform->SetPosition(glm::vec3(0.f, 0.f, sin(glfwGetTime()) * 10.f + 10.f));

		m_Renderer.Tick();

		// under current setup mesh drawing can be out of sync
		for (std::unique_ptr<CObject>& Object : Objects)
		{
			if (Object.get())
			{
				Object->TickComponents();
			}
		}

		ECS.RunComponentSystems();

		m_Renderer.PostTick();
	}

	Shutdown();
}
