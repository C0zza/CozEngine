#include "System.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Object.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Transform.h"
#include "Game/Components/TestComponent.h"
#include "Components/CameraComponent.h"
#include "Components/Lighting/DirectionalLightComponent.h"
#include "Components/Lighting/PointLightComponent.h"
#include "Components/Lighting/SpotLightComponent.h"
#include "Rendering/Material.h"
#include "Components/ModelComponent.h"
#include "Rendering/Model.h"

#include "Game/Components/TestECSComponent.h"

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
	//std::shared_ptr<LTexture> SmileyTexture = std::make_shared<LTexture>(LTexture("awesomeface.png", true, ETextureType::Diffuse));

	CObject* CameraObject = new CObject();
	CameraObject->Transform.Move(glm::vec3(0.f, 0.f, 10.f));
	CameraObject->Components.AddComponent<CTestComponent>(CameraObject);
	CCameraComponent* Camera = CameraObject->Components.AddComponent<CCameraComponent>(CameraObject);
	Camera->ActivateCamera();
	Objects.emplace_back(CameraObject);

	CObject* TestObject = new CObject();
	TestObject->Transform.Move(glm::vec3(0.f, 0.f, 0.f));
	Objects.emplace_back(TestObject);

	std::shared_ptr<LModel> Model = std::make_shared<LModel>("Game/Assets/backpack/backpack.obj");

	CModelComponent* TestModelComponent = TestObject->Components.AddComponent<CModelComponent>(TestObject);

	TestModelComponent->SetModel(Model);

	std::shared_ptr<LShader> DefaultShader = std::make_shared<LShader>("Engine/Rendering/DefaultShaders/shader.vs", "Engine/Rendering/DefaultShaders/shader.fs");

	CObject* DirectionalLightObject = new CObject();
	DirectionalLightObject->Transform.SetRotation(glm::vec3(0.f, 45.f, 0.f));
	Objects.emplace_back(DirectionalLightObject);
	CDirectionalLightComponent* DirectionalLight = DirectionalLightObject->Components.AddComponent<CDirectionalLightComponent>(DirectionalLightObject);
	DirectionalLight->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	DirectionalLight->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	DirectionalLight->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

	std::shared_ptr<LModel> CubeModel = std::make_shared<LModel>();

	CObject* SpotLightObject = new CObject();
	Objects.emplace_back(SpotLightObject);

	CModelComponent* CubeModelComponent = SpotLightObject->Components.AddComponent<CModelComponent>(SpotLightObject);
	CubeModelComponent->SetModel(CubeModel);

	std::shared_ptr<LMaterial> CubeMaterial = std::make_shared<LMaterial>(DefaultShader);
	std::shared_ptr<LTexture> BoxTexture = std::make_shared<LTexture>("container.jpg", false, ETextureType::Diffuse);
	CubeMaterial->Diffuse = BoxTexture;

	CubeModelComponent->SetMaterial(CubeMaterial);

	CSpotLightComponent* SpotLight = SpotLightObject->Components.AddComponent<CSpotLightComponent>(SpotLightObject);
	SpotLight->SetAmbient(glm::vec3(0.f, 0.2f, 0.f));
	SpotLight->SetDiffuse(glm::vec3(0.f, 0.5f, 0.f));
	SpotLight->SetSpecular(glm::vec3(0.f, 0.7f, 0.f));
	SpotLight->SetCutOff(glm::cos(glm::radians(12.5f)));
	SpotLight->SetOuterCutOff(glm::cos(glm::radians(17.5f)));
	SpotLight->SetConstant(1.0f);
	SpotLight->SetLinear(0.09f);
	SpotLight->SetQuadratic(0.032f);

	// TEMP
	PointLightTransform = &SpotLightObject->Transform;

	std::shared_ptr<LMaterial> DefaultMaterial = std::make_shared<LMaterial>(DefaultShader);
	std::shared_ptr<LTexture> BagTexture = std::make_shared<LTexture>("backpack/diffuse.jpg", false, ETextureType::Diffuse);
	std::shared_ptr<LTexture> SpecBagTexture = std::make_shared<LTexture>("backpack/specular.jpg", false, ETextureType::Specular);

	DefaultMaterial->Diffuse = BagTexture;
	DefaultMaterial->Specular = SpecBagTexture;
	DefaultMaterial->SpecularShininess = 32.f;

	TestModelComponent->SetMaterial(DefaultMaterial);

	InputManager.Init(m_Renderer.GetWindow().get());

	ECS.AddComponentSystem<CTestECSComponentSystem, CTestECSComponent>();

	TestEntity = new CTestEntity(&ECS);
}

void System::Run()
{
	assert(m_Renderer.GetWindow());
	while (!m_Renderer.GetWindow()->ShouldClose())
	{
		PointLightTransform->SetPosition(glm::vec3(0.f, 0.f, sin(glfwGetTime()) * 10.f + 10.f));

		CPointLightComponent::UpdatePointLights();
		CSpotLightComponent::UpdateSpotLights();
		CDirectionalLightComponent::UpdateDirectionalLight();

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
