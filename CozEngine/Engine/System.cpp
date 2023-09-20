#include "System.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Object.h"
#include "Camera.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Transform.h"
#include "Game/Components/TestComponent.h"
#include "Rendering/Material.h"
#include "Components/ModelComponent.h"
#include "Rendering/Model.h"

System::System()
{
}

System::~System()
{
}

void System::Init()
{
	m_Renderer.Init();
	m_InputManager.Init(m_Renderer.GetWindow());
}

void System::Shutdown()
{
	m_Renderer.Shutdown();
}

void System::SetupGame()
{
	LTexture::SetFlipVerticallyOnLoad(true);
	//std::shared_ptr<LTexture> SmileyTexture = std::make_shared<LTexture>(LTexture("awesomeface.png", true, ETextureType::Diffuse));

	SomeCamera = std::make_shared<Camera>();
	SomeCamera->CameraTransform->Move(glm::vec3(0.f, 0.f, 10.f));
	m_Renderer.SetActiveCamera(SomeCamera);

	CObject* TestObject = new CObject();
	TestObject->Components.AddComponent<CTestComponent>(TestObject);
	TestObject->Transform.Move(glm::vec3(0.f, 0.f, 0.f));
	Objects.emplace_back(TestObject);

	std::shared_ptr<LModel> Model = std::make_shared<LModel>("Game/Assets/backpack/backpack.obj");

	CModelComponent* TestModelComponent = TestObject->Components.AddComponent<CModelComponent>(TestObject);

	TestModelComponent->SetModel(Model);

	std::shared_ptr<LShader> DefaultShader = std::make_shared<LShader>("Engine/Rendering/DefaultShaders/shader.vs", "Engine/Rendering/DefaultShaders/shader.fs");
	DefaultShader->Use();
	DefaultShader->SetVec3("Light.Ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	DefaultShader->SetVec3("Light.Diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	DefaultShader->SetVec3("Light.Specular", glm::vec3(1.0f, 1.0f, 1.0f));

	std::shared_ptr<LMaterial> DefaultMaterial = std::make_shared<LMaterial>(DefaultShader);
	std::shared_ptr<LTexture> BoxTexture = std::make_shared<LTexture>("backpack/diffuse.jpg", false, ETextureType::Diffuse);
	std::shared_ptr<LTexture> SpecularBoxTexture = std::make_shared<LTexture>("backpack/specular.jpg", false, ETextureType::Specular);

	DefaultMaterial->Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	DefaultMaterial->Diffuse = BoxTexture;
	DefaultMaterial->Specular = SpecularBoxTexture;
	DefaultMaterial->SpecularShininess = 32.f;

	TestModelComponent->SetMaterial(DefaultMaterial);
}

void System::Run()
{
	float radius = 10.0f;

	// LShader::SetGlobalVec("Light.Position", glm::vec3(10.f, 0.f, 0.f));
	LShader::SetGlobalVec("Light.Direction", glm::vec3(-1.f, 0.f, 0.f));

	assert(m_Renderer.GetWindow());
	while (!m_Renderer.GetWindow()->ShouldClose())
	{
		m_InputManager.ProcessInput();

		m_Renderer.Tick();

		/*float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		SomeCamera->CameraTransform->SetPosition(glm::vec3(camX, 0.f, camZ));*/

		LShader::SetGlobalVec("ViewPos", SomeCamera->CameraTransform->GetPosition());
		LShader::SetGlobalMat4("View", m_Renderer.GetViewMatrix());
		LShader::SetGlobalMat4("Projection", m_Renderer.GetProjectionMatrix());

		// under current setup mesh drawing can be out of sync
		for (std::unique_ptr<CObject>& Object : Objects)
		{
			if (Object.get())
			{
				Object->TickComponents();
			}
		}

		m_Renderer.PostTick();
	}

	Shutdown();
}
