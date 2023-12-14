#include "ECSModelComponent.h"

#include "ECS/ECS.h"
#include "ECSTransformComponent.h"

void CECSModelComponentSystem::RunComponent(CECSModelComponent& Component)
{
	if (Component.Model.get() && Component.Material.get())
	{
		// Only need the one Material->Use call while we have 1 mat per CModelComponent
		Component.Material->Use();
		assert(Component.Material->GetShader());

		if (CECSTransformComponent* EntityTransform = LECS::Get()->GetComponent<CECSTransformComponent>(Component.EntityID))
		{
			Component.Model->Draw(*Component.Material->GetShader(), EntityTransform->GetUpdatedTransformationMatrix());
		}
	}
}
