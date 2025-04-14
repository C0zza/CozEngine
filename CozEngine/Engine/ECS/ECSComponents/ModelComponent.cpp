#include "ModelComponent.h"

#include "ECS/ECS.h"
#include "Globes.h"
#include "Rendering/Renderer.h"
#include "TransformComponent.h"

#if defined(COZ_EDITOR)
#include "Editor/DrawModeSubsystem.h"
#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Editor/SelectedEntitySubsystem.h"
#endif

void CModelComponentSystem::RunComponent(CModelComponent& Component)
{
	if (Component.Model.Get() && Component.ModelMaterial.Get())
	{
		// Only need the one Material->Use call while we have 1 mat per CModelComponent
#if defined(COZ_EDITOR)
		Component.ModelMaterial->EntityID = Component.EntityID;

		const bool bDrawOutline = SelectedEntitySubsystem ? SelectedEntitySubsystem->GetSelectedEntityID() == Component.EntityID : false 
									&& Component.ModelMaterial->HasShaderForDrawMode(EDrawMode::Outline) 
									&& DrawModeSubsystem ? DrawModeSubsystem->GetActiveDrawMode() == EDrawMode::Default : false;

		if (bDrawOutline)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}
#endif
		
		assert(ECS);
		CTransformComponent* EntityTransform = ECS->GetComponent<CTransformComponent>(Component.EntityID);

		auto DrawFunc = [this, EntityTransform, &Component](const LShader* Shader)
		{
			if (EntityTransform && Shader)
			{
				Component.Model->Draw(*Shader, EntityTransform->GetUpdatedTransformationMatrix());
			}
		};

		DrawFunc(Component.ModelMaterial->Use());

#if defined(COZ_EDITOR)
		if (bDrawOutline)
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			if (EntityTransform)
			{
				const glm::vec3 TempScale = EntityTransform->GetScale();
				EntityTransform->ScaleByFloat(1.05f);

				const EDrawMode CachedDrawMode = DrawModeSubsystem->GetActiveDrawMode();

				DrawModeSubsystem->SetActiveDrawMode(EDrawMode::Outline);
				DrawFunc(Component.ModelMaterial->Use());
				DrawModeSubsystem->SetActiveDrawMode(CachedDrawMode);

				EntityTransform->SetScale(TempScale);
			}

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
		}
#endif
	}
}

#if defined(COZ_EDITOR)
void CModelComponentSystem::DrawImGuiComponent(CModelComponent& Component)
{
	LImGuiPropertyDrawHelpers::DrawProperty("Model", Component.Model);
	LImGuiPropertyDrawHelpers::DrawProperty("Model Material", Component.ModelMaterial);
}

void CModelComponentSystem::Init()
{
	SelectedEntitySubsystem = CSystem.GetSubsystems().GetSubsystem<LSelectedEntitySubsystem>();
	if (!SelectedEntitySubsystem)
	{
		Log(LLogLevel::ERROR, "CModelComponentSystem::Init - Invalid SelectedEntitySubsystem");
	}

	DrawModeSubsystem = CSystem.GetSubsystems().GetSubsystem<LDrawModeSubsystem>();
	if (!DrawModeSubsystem)
	{
		Log(LLogLevel::ERROR, "CModelComponentSystem::Init - Invalid DrawModeSubsystem");
	}
}
#endif

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


