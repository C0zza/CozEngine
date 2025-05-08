#include "stdafx.h"

#include "AssetEditorWindow.h"

#include "Editor/AssetEditorWindowSubsystem.h"
#include "Globes.h"

void LAssetEditorWindow::Draw()
{
	LAssetEditorWindowSubsystem* AssetEditorWindowSubsystem = CSystem.GetSubsystems().GetSubsystem<LAssetEditorWindowSubsystem>();
	if (!AssetEditorWindowSubsystem)
	{
		Log(LLogLevel::ERROR, "LAssetEditorWindow::Draw - Invalid AssetEditorWindowSubsystem.");
		return;
	}

	// Copy so we can unregister without worrying about removing elements during iteration
	const std::set<std::filesystem::path> Assets = AssetEditorWindowSubsystem->GetRegisteredAssets();

	if (ImGui::BeginTabBar("AssetTabBar", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable))
	{
		for (const std::filesystem::path& Asset : Assets)
		{
			bool Open = true;

			if (ImGui::BeginTabItem(Asset.string().c_str(), &Open, ImGuiTabItemFlags_None))
			{
				ImGui::Text(Asset.string().c_str());
				ImGui::EndTabItem();
			}

			if (!Open)
			{
				AssetEditorWindowSubsystem->UnregisterAsset(Asset);
			}
		}

		ImGui::EndTabBar();
	}
}
