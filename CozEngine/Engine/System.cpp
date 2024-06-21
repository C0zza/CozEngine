#include "System.h"

#include <utility>

#include "ECS/ECS.h"
#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Editor/LEditor.h"
#include "Game/Components/Movement.h"
#include "Game/DirectionalLightEntity.h"
#include "Game/PlayerEntity.h"
#include "Game/SpotLightEntity.h"
#include "Game/TestEntity.h"
#include "Input/InputManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

void LSystem::Run()
{
	LRenderer* Renderer = Subsystems.GetSubsystem<LRenderer>(true);

	// TODO: Must be a better way than registering everything here. Maybe check if it needs adding when the corresponding component is added?
	LECS* ECS = Subsystems.GetSubsystem<LECS>(true);
	ECS->AddComponentSystem<CModelComponentSystem, CModelComponent>();
	ECS->AddComponentSystem<LComponentSystem<CTransformComponent>, CTransformComponent>();
	ECS->AddComponentSystem<LComponentSystem<CCameraComponent>, CCameraComponent>();
	ECS->AddComponentSystem<LComponentSystem<CSpotLightComponent>, CSpotLightComponent>();
	ECS->AddComponentSystem<LComponentSystem<CPointLightComponent>, CPointLightComponent>();
	ECS->AddComponentSystem<LComponentSystem<CDirectionalLightComponent>, CDirectionalLightComponent>();
	ECS->AddComponentSystem<CMovementSystem, CMovement>();

	// TODO: No system cleaning entities up on shutdown
	LEntity* TestEntity = new CTestEntity();
	LEntity* CameraEntity = new CPlayerEntity();
	LEntity* SpotLightEntity = new CSpotLightEntity();
	LEntity* DirectionalLightEntity = new CDirectionalLightEntity();

#if defined(COZ_EDITOR)
	LEditor* Editor = Subsystems.AddSubsystem<LEditor>();
#endif

	assert(Renderer->GetWindow());
	while (!Renderer->GetWindow()->ShouldClose())
	{
		Subsystems.ForEachSubsystem([](LSubsystem* Subsystem)
			{
				Subsystem->PreTick();
			});

		Renderer->Update();

		Subsystems.ForEachSubsystem([](LSubsystem* Subsystem)
			{
				Subsystem->Tick();
			});

		Subsystems.ForEachSubsystem([](LSubsystem* Subsystem)
			{
				Subsystem->PostTick();
			});

		Renderer->Swap();
	}
}
