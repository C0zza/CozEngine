#include "stdafx.h"

#include "SceneOutlinerWindow.h"

#include "ECS/ECS.h"
#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"
#include "WorldManager.h"

LSceneOutlinerWindow::LSceneOutlinerWindow(const char* WindowName)
	: LEditorWindow(WindowName)
{
	WorldManager = CSystem.GetSubsystems().GetSubsystem<LWorldManager>();
	assert(WorldManager);

	ECS = CSystem.GetSubsystems().GetSubsystem<LECS>();
	assert(ECS);

	SelectedEntitySubsystem = CSystem.GetSubsystems().GetSubsystem<LSelectedEntitySubsystem>();
	assert(SelectedEntitySubsystem);
}

void LSceneOutlinerWindow::DrawWindow()
{
	const std::map<LEntityID, std::unique_ptr<LEntity>>& Entities = WorldManager->GetActiveWorld()->EntityContainer.GetEntities();

	const LEntityID SelectedID = SelectedEntitySubsystem->GetSelectedEntityID();

	for (const std::pair<const LEntityID, std::unique_ptr<LEntity>>& Pair : Entities)
	{
		const LEntityID EntityID = Pair.first;
		LEntity* Entity = Pair.second.get();

		if (ImGui::Selectable(std::to_string(EntityID).c_str(), EntityID == SelectedID))
		{
			SelectedEntitySubsystem->SetSelectedEntityID(EntityID);
		}
	}
}
