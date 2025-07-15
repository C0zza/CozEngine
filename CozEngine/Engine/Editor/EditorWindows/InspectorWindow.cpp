#include "stdafx.h"
#include "InspectorWindow.h"

#include <string>

#include "ECS/ECS.h"
#include "ECS/ECS2/ArchetypeManager.h"
#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"
#include "json.hpp"
#include "Reflection/Class.h"

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

	LArchetypeManager* ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
	if (!ArchetypeManager)
	{
		Log(LLogLevel::ERROR, "LInspectorWindow::Draw - Invalid ArchetypeManager");
		return;
	}

	ImGui::BeginChild("Selected Entities");
	{
		const LEntityID SelectedEntityID = SelectedEntitySubsystem->GetSelectedEntityID();

		ArchetypeManager->ForEachEntityComponent(SelectedEntityID, [SelectedEntityID](char* ComponentAddress, LClass* ComponentClass)
			{
				if (ImGui::TreeNode(ComponentClass->GetTypeName().data()))
				{
					ComponentClass->DrawEditorDetails(static_cast<void*>(ComponentAddress));
					
					ImGui::TreePop();
				}
			});

		ImGui::EndChild();
	}
}

