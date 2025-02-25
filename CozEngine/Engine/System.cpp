#include "System.h"

#include <utility>

#include "Development/LImGuiSubsystem.h"
#include "ECS/ECS.h"
#include "Rendering/FrameBufferSubsystem.h"
#include "Rendering/Renderer.h"

#if defined(COZ_EDITOR)
#include "Editor/LEditor.h"
#endif

#include "Rendering/CubeMap.h"

#include "ECS/EntityManagement/World.h"

#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Game/Components/MovementComponent.h"

void LSystem::Run()
{
	LResourceManager* ResourceManager = Subsystems.GetSubsystem<LResourceManager>(true);
	LRenderer* Renderer = Subsystems.GetSubsystem<LRenderer>(true);
#if defined(COZ_EDITOR)
	LImGuiSubsystem* ImGuiSubsystem = Subsystems.GetSubsystem<LImGuiSubsystem>(true);
	LFrameBufferSubsystem* FrameBufferSubsystem = Subsystems.GetSubsystem<LFrameBufferSubsystem>(true);
#endif

	// TODO: Must be a better way than registering everything here. Maybe check if it needs adding when the corresponding component is added?
	LECS* ECS = Subsystems.GetSubsystem<LECS>(true);
	ECS->AddComponentSystem<CModelComponentSystem, CModelComponent>();
	ECS->AddComponentSystem<CTransformComponentSystem, CTransformComponent>();
	ECS->AddComponentSystem<CCameraComponentSystem, CCameraComponent>();
	ECS->AddComponentSystem<CSpotLightComponentSystem, CSpotLightComponent>();
	ECS->AddComponentSystem<CPointLightComponentSystem, CPointLightComponent>();
	ECS->AddComponentSystem<CDirectionalLightComponentSystem, CDirectionalLightComponent>();
	ECS->AddComponentSystem<CMovementSystem, CMovementComponent>();
	ECS->AddComponentSystem<CLandscapeComponentSystem, CLandscapeComponent>();

	std::unique_ptr<LCubeMap> TestCubeMap = std::make_unique<LCubeMap>("Game/Content/Skybox.casset", "Game/Content/Models/MOD_Cube.casset");

	std::unique_ptr<LWorld> World = std::make_unique<LWorld>("Game/Content/Levels/TestLevel.casset");

#if defined(COZ_EDITOR)
	LEditor* Editor = Subsystems.AddSubsystem<LEditor>();
#endif

	assert(Renderer);
	bool bShouldWindowClose = true;
	Renderer->GetShouldWindowClose(bShouldWindowClose);

	while (!bShouldWindowClose)
	{

		Renderer->Update();

#if defined(COZ_EDITOR)
		ImGuiSubsystem->NewFrame();
		Renderer->BindFrameBuffer(Editor->GetSceneFrameBuffer());
#endif
		Renderer->ClearFrameBuffer(.0f, .0f, .0f, 1.f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		TestCubeMap->Draw();
		ECS->Update();

#if defined(COZ_EDITOR)
		Renderer->BindDefaultFrameBuffer();
		Renderer->ClearFrameBuffer(.0f, .0f, .0f, 1.f, GL_COLOR_BUFFER_BIT);

		// Something like this will be needed for post processing, but for now the editor is drawn inside an imgui window via LEditor::Draw
		//FrameBufferSubsystem->DrawFrameBuffer(Editor->GetSceneFrameBuffer());

		Editor->Draw();
		ImGuiSubsystem->Render();
#endif

		Renderer->Swap();

		Renderer->GetShouldWindowClose(bShouldWindowClose);
	}
}
