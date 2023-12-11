#include "ECSModelComponent.h"

void CECSModelComponentSystem::RunComponent(CECSModelComponent& Component)
{
	if (Component.Model.get() && Component.Material.get() && Component.EntityTransform)
	{
		// Only need the one Material->Use call while we have 1 mat per CModelComponent
		Component.Material->Use();
		assert(Component.Material->GetShader());

		Component.Model->Draw(*Component.Material->GetShader(), Component.EntityTransform->GetUpdatedTransformationMatrix());
	}
}
