#include "System.h"

#include <utility>

#include "ECS/ECS.h"
#include "Rendering/Renderer.h"

#if defined(COZ_EDITOR)
#include "Editor/LEditor.h"
#endif

#include "ECS/ECSComponents/ECSComponentHeaders.h"
#include "Game/Components/Movement.h"
#include "Game/DirectionalLightEntity.h"
#include "Game/PlayerEntity.h"
#include "Game/SpotLightEntity.h"
#include "Game/TestEntity.h"
#include "Game/LandscapeEntity.h"

void LSystem::Run()
{
	LRenderer* Renderer = Subsystems.GetSubsystem<LRenderer>(true);

	// TODO: Must be a better way than registering everything here. Maybe check if it needs adding when the corresponding component is added?
	LECS* ECS = Subsystems.GetSubsystem<LECS>(true);
	ECS->AddComponentSystem<CModelComponentSystem, CModelComponent>();
	ECS->AddComponentSystem<LComponentSystem<CTransformComponent>, CTransformComponent>();
	ECS->AddComponentSystem<CCameraComponentSystem, CCameraComponent>();
	ECS->AddComponentSystem<CSpotLightComponentSystem, CSpotLightComponent>();
	ECS->AddComponentSystem<CPointLightComponentSystem, CPointLightComponent>();
	ECS->AddComponentSystem<CDirectionalLightComponentSystem, CDirectionalLightComponent>();
	ECS->AddComponentSystem<CMovementSystem, CMovement>();
	ECS->AddComponentSystem<CLandscapeComponentSystem, CLandscapeComponent>();

	// TODO: No system cleaning entities up on shutdown
	LEntity* LandscapeEntity = new CLandscapeEntity();
	LEntity* TestEntity = new CTestEntity();
	LEntity* CameraEntity = new CPlayerEntity();
	LEntity* SpotLightEntity = new CSpotLightEntity();
	LEntity* DirectionalLightEntity = new CDirectionalLightEntity();

#if defined(COZ_EDITOR)
	LEditor* Editor = Subsystems.AddSubsystem<LEditor>();
#endif

	assert(Renderer);
	bool bShouldWindowClose = true;
	Renderer->GetShouldWindowClose(bShouldWindowClose);

	while (!bShouldWindowClose)
	{
		Renderer->Update();
		ECS->Update();
		Renderer->Swap();

		Renderer->GetShouldWindowClose(bShouldWindowClose);
	}
}
