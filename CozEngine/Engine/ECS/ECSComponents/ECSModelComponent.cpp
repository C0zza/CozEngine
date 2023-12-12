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

		CECSTransformComponent EntityTransform;
		if (ECS->GetComponent<CECSTransformComponent>(Component.EntityID, EntityTransform))
		{
			// TODO: BAD, we're getting a reference to the transform. Updating the matrix if necessary, then losing that calculation
			Component.Model->Draw(*Component.Material->GetShader(), EntityTransform.GetUpdatedTransformationMatrix());
		}
	}
}
