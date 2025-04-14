#include "System.h"

#include <utility>

#include "Development/LImGuiSubsystem.h"
#include "ECS/ECS.h"
#include "Rendering/FrameBufferSubsystem.h"
#include "Rendering/Renderer.h"

#if defined(COZ_EDITOR)
#include "Editor/DrawModeSubsystem.h"
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
	LEditor* Editor = Subsystems.AddSubsystem<LEditor>();
	LDrawModeSubsystem* DrawModeSubsystem = Subsystems.GetSubsystem<LDrawModeSubsystem>();

	glEnable(GL_STENCIL_TEST);
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

	assert(Renderer);
	bool bShouldWindowClose = true;
	Renderer->GetShouldWindowClose(bShouldWindowClose);

	while (!bShouldWindowClose)
	{
		Renderer->Update();

#if defined(COZ_EDITOR)
		ImGuiSubsystem->NewFrame();
		Renderer->BindFrameBuffer(Editor->GetSceneFrameBuffer());
		
		// stencil buffer doesn't clear properly if the mask is 0x00
		glStencilMask(0xFF);
#endif
		Renderer->ClearFrameBuffer(.0f, .0f, .0f, 1.f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#if defined(COZ_EDITOR)
		glStencilMask(0x00);
#endif

		glEnable(GL_DEPTH_TEST);

		TestCubeMap->Draw();
		ECS->UpdateComponentSystemTypes(EComponentSystemType::Ticker);
		ECS->UpdateComponentSystemTypes(EComponentSystemType::Renderer);

#if defined(COZ_EDITOR)
		// TODO: This editor side frame buffer management feels like it should be abstraced into Editor->Draw somehow

		Renderer->BindFrameBuffer(Editor->GetEntityFrameBuffer());
		// Cleared to 0,0,0,0 as we'll use no color as no object. LTypeIdGenerator has been updated to start ID counts from 1.
		Renderer->ClearFrameBuffer(.0f, .0f, .0f, 0.f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawModeSubsystem->SetActiveDrawMode(EDrawMode::EntityFrameBuffer);
		ECS->UpdateComponentSystemTypes(EComponentSystemType::Renderer);
		DrawModeSubsystem->SetActiveDrawMode(EDrawMode::Default);

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
