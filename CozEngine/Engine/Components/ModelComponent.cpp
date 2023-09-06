#include "ModelComponent.h"

#include "Rendering/Model.h"
#include "Object.h"
#include "Transform.h"

void CModelComponent::Tick()
{
	if (Model.get() && Material.get())
	{
		assert(Parent);
		Model->Draw(*Material.get(), Parent->Transform);
	}
}
