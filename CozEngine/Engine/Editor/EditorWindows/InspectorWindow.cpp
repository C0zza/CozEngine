#include "stdafx.h"
#include "InspectorWindow.h"

#include <string>

#include "ECS/ECS.h"
#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"
#include "json.hpp"

void LInspectorWindow::DrawWindow()
{
	LSelectedEntitySubsystem* SelectedEntitySubsystem = CSystem.GetSubsystems().GetSubsystem<LSelectedEntitySubsystem>();
	if (!SelectedEntitySubsystem)
	{
		Log(LLogLevel::ERROR, "LInspectorWindow - Invalid SelectedEntitySubsystem.");
		return;
	}

	if (!SelectedEntitySubsystem->IsEntitySelected())
	{
		return;
	}

	LECS* ECS = ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	if (!ECS)
	{
		Log(LLogLevel::ERROR, "LInspectorWindow::Draw - Invalid ECS.");
		return;
	}

	ImGui::BeginChild("Selected Entities");
	{
		const LEntityID SelectedEntityID = SelectedEntitySubsystem->GetSelectedEntityID();

		nlohmann::json EntityJson;
		ECS->GetEntityComponentData(SelectedEntityID, EntityJson);
		for (const auto & [Key, Value] : EntityJson.items())
		{
			LComponentSystemBase* ComponentSystem = ECS->GetComponentSystemByName(Key);
			if (!ComponentSystem)
			{
				// TODO: Add 1 time log macro for cases where they may spam.
				continue;
			}

			if (ImGui::TreeNode(Key.c_str()))
			{
// TODO: If editor code was in it's own module we wouldn't need this
#if defined(COZ_EDITOR)
				ComponentSystem->DrawImGuiComponent(SelectedEntityID);
#endif
				ImGui::TreePop();
			}
			ImGui::Spacing();
		}

		ImGui::EndChild();
	}
}

