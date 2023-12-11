#include "ModelComponent.h"

#include "Rendering/Model.h"
#include "Object.h"
#include "Transform.h"

void CModelComponent::Tick()
{
	if (Model.get() && Material.get())
	{
		// Only need the one Material->Use call while we have 1 mat per CModelComponent
		Material->Use();
		assert(Parent);
		assert(Material->GetShader());

		if (Parent->Transform.GetIsDirty())
		{
			Parent->Transform.UpdateTransformationMatrix();
		}

		Model->Draw(*Material->GetShader(), Parent->Transform.GetUpdatedTransformationMatrix());
	}
}
