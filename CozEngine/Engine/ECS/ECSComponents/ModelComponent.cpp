#include "ModelComponent.h"

#include "ECS/ECS.h"
#include "Globes.h"
#include "Rendering/Renderer.h"
#include "TransformComponent.h"

void CModelComponentSystem::RunComponent(CModelComponent& Component)
{
	if (Component.Model.Get() && Component.ModelMaterial.Get())
	{
		// Only need the one Material->Use call while we have 1 mat per CModelComponent
		const LShader* ActiveShader = Component.ModelMaterial->Use();
		if (!ActiveShader)
		{
			return;
		}
		
		assert(ECS);
		if (CTransformComponent* EntityTransform = ECS->GetComponent<CTransformComponent>(Component.EntityID))
		{
			Component.Model->Draw(*ActiveShader, EntityTransform->GetUpdatedTransformationMatrix());
		}
	}
}

void CModelComponentSystem::GetSerializedComponent(const CModelComponent& Component, nlohmann::json& J) const
{
	J["Model"] = Component.Model;
	J["ModelMaterial"] = Component.ModelMaterial;
}

void CModelComponentSystem::DeserializeComponent(CModelComponent& Component, const nlohmann::json& J)
{
	Component.Model = J["Model"];
	Component.ModelMaterial = J["ModelMaterial"];
}


