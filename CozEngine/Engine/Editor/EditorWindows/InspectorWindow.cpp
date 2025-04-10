#include "stdafx.h"
#include "InspectorWindow.h"

#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"

#include <functional>

LInspectorWindow::LInspectorWindow(const char* iWindowName)
	: LEditorWindow(iWindowName)
{
	LSelectedEntitySubsystem* SelectedEntitySubsystem = CSystem.GetSubsystems().GetSubsystem<LSelectedEntitySubsystem>();
	if (!SelectedEntitySubsystem)
	{
		Log(LLogLevel::ERROR, "LInspectorWindow::LInspectorWindow - Invalid SelectedEntitySubsystem.");
		return;
	}

	SelectedEntitySubsystem->GetOnEntitySelectedDelegate().AddEvent(this, &LInspectorWindow::OnEntitySelected, OnEntitySelectedHandle);
}

void LInspectorWindow::Draw()
{
}

void LInspectorWindow::OnEntitySelected(const LEntityID EntityID)
{
	Log(LLogLevel::INFO, "Entity ID " + std::to_string(EntityID) + " has been selected.");
}

