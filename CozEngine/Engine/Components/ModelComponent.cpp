#include "ModelComponent.h"

#include "Rendering/Mesh.h"
#include "Object.h"
#include "Transform.h"

void CModelComponent::Tick()
{
	if (Mesh.get() && Material.get())
	{
		assert(Parent);
		Mesh->Draw(*Material.get(), Parent->Transform);
	}
}
