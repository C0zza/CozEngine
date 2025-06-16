#include "stdafx.h"

#include "SettingsWindow.h"

#include "Settings/SettingsManager.h"
#include "Globes.h"
#include "Reflection/Class.h"

LSettingsWindow::LSettingsWindow(const char* WindowName)
	: LEditorWindow(WindowName)
{
	SettingsManager = CSystem.GetSubsystems().GetSubsystem<LSettingsManager>(true);
	assert(SettingsManager);
}

void LSettingsWindow::DrawWindow()
{
	if (ImGui::BeginTable("##SettingsWindowTable", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();

		SettingsManager->ForEachSettingsClass([this](LClass* SettingsClass, LSettings* SettingsObject)
			{
				assert(SettingsClass);
				assert(SettingsObject);

				const std::string SettingsClassName = SettingsClass->GetTypeName().data();

				if (ImGui::Button(SettingsClassName.data()))
				{
					SelectedSettingsClass = SettingsClass;
					SelectedSettingsObject = SettingsObject;
				}
			});

		ImGui::TableNextColumn();

		if (SelectedSettingsClass && SelectedSettingsObject)
		{
			if (ImGui::Button("Save"))
			{
				SettingsManager->SaveSettings(SelectedSettingsClass);
			}

			SelectedSettingsClass->DrawEditorDetails(SelectedSettingsObject);
		}
		else
		{
			ImGui::Text("Select a settings class on the left to view default state.");
		}

		ImGui::EndTable();
	}
}
