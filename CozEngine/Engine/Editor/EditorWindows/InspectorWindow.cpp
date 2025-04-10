#include "stdafx.h"
#include "InspectorWindow.h"

#include <string>

#include "ECS/ECS.h"
#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"
#include "json.hpp"

void LInspectorWindow::Draw()
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
		if (ImGui::TreeNode(std::string("Entity - " + std::to_string(SelectedEntityID)).c_str()))
		{
			for (const auto & [Key, Value] : EntityJson.items())
			{
				if (ImGui::TreeNode(Key.c_str()))
				{
					for (const auto& [CompKey, CompValue] : Value.items())
					{
						if (ImGui::TreeNode(CompKey.c_str()))
						{
							for (const auto& [ValueKey, Value] : CompValue.items())
							{
								// TODO: Need a better way of drawing components to editor
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
				ImGui::Spacing();
			}
			ImGui::TreePop();
		}

		ImGui::EndChild();
	}
}

