#include "ModelComponent.h"

#include "ECS/ECS.h"
#include "TransformComponent.h"

void CModelComponentSystem::RunComponent(CModelComponent& Component)
{
	if (Component.Model.get() && Component.Material.get())
	{
		// Only need the one Material->Use call while we have 1 mat per CModelComponent
		Component.Material->Use();
		assert(Component.Material->GetShader());

		if (CTransformComponent* EntityTransform = LECS::Get()->GetComponent<CTransformComponent>(Component.EntityID))
		{
			Component.Model->Draw(*Component.Material->GetShader(), EntityTransform->GetUpdatedTransformationMatrix());
		}
	}
}
