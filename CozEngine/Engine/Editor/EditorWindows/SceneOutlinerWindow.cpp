#include "stdafx.h"

#include "SceneOutlinerWindow.h"

#include "ECS/ECS2/ArchetypeManager.h"
#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"

LSceneOutlinerWindow::LSceneOutlinerWindow(const char* WindowName)
	: LEditorWindow(WindowName)
{
	SelectedEntitySubsystem = CSystem.GetSubsystems().GetSubsystem<LSelectedEntitySubsystem>();
	assert(SelectedEntitySubsystem);

	ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
	assert(ArchetypeManager);
}

void LSceneOutlinerWindow::DrawWindow()
{
	std::vector<LEntityID> EntityIDs = ArchetypeManager->GetAllEntityIDs();

	const LEntityID SelectedID = SelectedEntitySubsystem->GetSelectedEntityID();

	for (const LEntityID EntityID : EntityIDs)
	{
		if (ImGui::Selectable(std::to_string(EntityID).c_str(), EntityID == SelectedID))
		{
			SelectedEntitySubsystem->SetSelectedEntityID(EntityID);
		}
	}
}
