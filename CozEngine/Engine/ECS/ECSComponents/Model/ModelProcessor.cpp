#include "stdafx.h"

#include "ModelProcessor.h"

#include "ECS/ECSComponents/ModelComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"

#if defined(COZ_EDITOR)
#include "Editor/DrawModeSubsystem.h"
#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Editor/SelectedEntitySubsystem.h"
#endif

void LModelProcessor::Initialize()
{
#if defined(COZ_EDITOR)
	SelectedEntitySubsystem = CSystem.GetSubsystems().GetSubsystem<LSelectedEntitySubsystem>(true);
	if (!SelectedEntitySubsystem)
	{
		Log(LLogLevel::ERROR, "CModelComponentSystem::Init - Invalid SelectedEntitySubsystem");
	}

	DrawModeSubsystem = CSystem.GetSubsystems().GetSubsystem<LDrawModeSubsystem>();
	if (!DrawModeSubsystem)
	{
		Log(LLogLevel::ERROR, "CModelComponentSystem::Init - Invalid DrawModeSubsystem");
	}
#endif

	std::set<TSubclassOf<LECSComponent>>& Components = Config.GetComponentSet();

	Components.insert(CTransformComponent::StaticClass());
	Components.insert(CModelComponent::StaticClass());

	Type = EEntityProcessorType::Render;
}

void LModelProcessor::Execute(FEntityQueryResult& EntityQueryResult)
{
	EntityQueryResult.ForEachEntityChunk([this](FEntityChunkHandle& EntityChunk)
		{
			std::span<const LEntityID> IDs = EntityChunk.GetEntityIDs();
			std::span<CModelComponent> Models = EntityChunk.GetComponentSpan<CModelComponent>();
			std::span<CTransformComponent> Transforms = EntityChunk.GetComponentSpan<CTransformComponent>();

			const int EntityCount = EntityChunk.GetEntityCount();

			for (int i = 0; i < EntityCount; ++i)
			{
				const LEntityID& ID = IDs[i];
				CModelComponent& ModelComponent = Models[i];
				CTransformComponent& TransformComponent = Transforms[i];

				if (!ModelComponent.Model.Get() || !ModelComponent.ModelMaterial.Get())
				{
					continue;
				}

#if defined(COZ_EDITOR)
				ModelComponent.ModelMaterial->EntityID = ID;

				const bool bDrawOutline = SelectedEntitySubsystem ? SelectedEntitySubsystem->GetSelectedEntityID() == ID : false
					&& ModelComponent.ModelMaterial->HasShaderForDrawMode(EDrawMode::Outline)
					&& DrawModeSubsystem ? DrawModeSubsystem->GetActiveDrawMode() == EDrawMode::Default : false;

				if (bDrawOutline)
				{
					glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
					glStencilFunc(GL_ALWAYS, 1, 0xFF);
					glStencilMask(0xFF);
				}
#endif

				// TODO: Move to method
				auto DrawFunc = [this, &TransformComponent, &ModelComponent](const LShader* Shader)
					{
						if (Shader)
						{
							ModelComponent.Model->Draw(*Shader, TransformComponent.GetUpdatedTransformationMatrix());
						}
					};

				DrawFunc(ModelComponent.ModelMaterial->Use());

#if defined(COZ_EDITOR)
				if (bDrawOutline)
				{
					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
					glStencilMask(0x00);

					const glm::vec3 TempScale = TransformComponent.GetScale();
					TransformComponent.ScaleByFloat(1.05f);

					const EDrawMode CachedDrawMode = DrawModeSubsystem->GetActiveDrawMode();

					DrawModeSubsystem->SetActiveDrawMode(EDrawMode::Outline);
					DrawFunc(ModelComponent.ModelMaterial->Use());
					DrawModeSubsystem->SetActiveDrawMode(CachedDrawMode);

					TransformComponent.SetScale(TempScale);

					glStencilMask(0xFF);
					glStencilFunc(GL_ALWAYS, 1, 0xFF);
				}
#endif
			}
		});
}

void LModelProcessor::DrawModel(CModelComponent& ModelComponent, CTransformComponent& TransformComponent, const LShader* Shader)
{
	if (Shader)
	{
		ModelComponent.Model->Draw(*Shader, TransformComponent.GetUpdatedTransformationMatrix());
	}
}
