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

#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Game/Components/MovementComponent.h"
#include "Game/DirectionalLightEntity.h"
#include "Game/PlayerEntity.h"
#include "Game/SpotLightEntity.h"
#include "Game/TestEntity.h"
#include "Game/LandscapeEntity.h"

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
	ECS->AddComponentSystem<LComponentSystem<CTransformComponent>, CTransformComponent>();
	ECS->AddComponentSystem<CCameraComponentSystem, CCameraComponent>();
	ECS->AddComponentSystem<CSpotLightComponentSystem, CSpotLightComponent>();
	ECS->AddComponentSystem<CPointLightComponentSystem, CPointLightComponent>();
	ECS->AddComponentSystem<CDirectionalLightComponentSystem, CDirectionalLightComponent>();
	ECS->AddComponentSystem<CMovementSystem, CMovementComponent>();
	ECS->AddComponentSystem<CLandscapeComponentSystem, CLandscapeComponent>();

	std::unique_ptr<LCubeMap> TestCubeMap = std::make_unique<LCubeMap>("Game/Content/Skybox.casset", "Game/Content/Models/MOD_Cube.casset");

	// TODO: No system cleaning entities up on shutdown. Will eventually be handled by the "world"
	std::unique_ptr<LEntity> LandscapeEntity = std::make_unique<CLandscapeEntity>();
	std::unique_ptr<LEntity> TestEntity = std::make_unique<CTestEntity>();
	std::unique_ptr<LEntity> CameraEntity = std::make_unique<CPlayerEntity>();
	std::unique_ptr<LEntity> SpotLightEntity = std::make_unique<CSpotLightEntity>();
	std::unique_ptr<LEntity> DirectionalLightEntity = std::make_unique<CDirectionalLightEntity>();

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
