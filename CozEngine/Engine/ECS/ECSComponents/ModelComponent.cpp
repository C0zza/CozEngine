#include "ModelComponent.h"

#include "ECS/ECS.h"
#include "Globes.h"
#include "Rendering/Renderer.h"
#include "TransformComponent.h"

void CModelComponentSystem::PreRun()
{
	if (LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>())
	{
		Renderer->BindCustomFrameBuffer();
	}
}

void CModelComponentSystem::PostRun()
{
	if (LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>())
	{
		Renderer->UnbindCustomFrameBuffer();
	}
}

void CModelComponentSystem::RunComponent(CModelComponent& Component)
{
	if (Component.Model.Get() && Component.Material.Get())
	{
		// Only need the one Material->Use call while we have 1 mat per CModelComponent
		Component.Material->Use();
		assert(Component.Material->GetShader());
		
		assert(ECS);
		if (CTransformComponent* EntityTransform = ECS->GetComponent<CTransformComponent>(Component.EntityID))
		{
			Component.Model->Draw(*Component.Material->GetShader(), EntityTransform->GetUpdatedTransformationMatrix());
		}
	}
}


