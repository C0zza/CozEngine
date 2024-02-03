#include "ModelComponent.h"

#include "ECS/ECS.h"
#include "TransformComponent.h"

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
