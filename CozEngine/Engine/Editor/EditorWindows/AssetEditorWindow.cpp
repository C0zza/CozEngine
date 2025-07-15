#include "stdafx.h"

#include "AssetEditorWindow.h"

#include "Editor/AssetEditorWindowSubsystem.h"
#include "Globes.h"
#include "Reflection/Class.h"
#include "ResourceManagement/ResourceManager.h"

void LAssetEditorWindow::DrawWindow()
{
	LAssetEditorWindowSubsystem* AssetEditorWindowSubsystem = CSystem.GetSubsystems().GetSubsystem<LAssetEditorWindowSubsystem>();
	if (!AssetEditorWindowSubsystem)
	{
		Log(LLogLevel::ERROR, "LAssetEditorWindow::Draw - Invalid AssetEditorWindowSubsystem.");
		return;
	}

	// Copy so we can unregister without worrying about removing elements during iteration
	std::unordered_map<std::filesystem::path, FAssetInfo> Assets = AssetEditorWindowSubsystem->GetRegisteredAssets();

	if (ImGui::BeginTabBar("AssetTabBar", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable))
	{
		for (auto& Asset : Assets)
		{
			const std::filesystem::path& Path = Asset.first;
			FAssetInfo& AssetInfo = Asset.second;

			bool Open = true;

			if (ImGui::BeginTabItem(Path.string().c_str(), &Open, ImGuiTabItemFlags_None))
			{
				if (AssetInfo.Class)
				{
					if (ImGui::Button("Save") && AssetInfo.Resource.Get())
					{
						LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
						ResourceManager->SaveResource(AssetInfo.Resource.Get()->GetAssetPath(), AssetInfo.Resource, AssetInfo.Class);
					}

					AssetInfo.Class->DrawEditorDetails(AssetInfo.Resource.Get());
				}
				ImGui::EndTabItem();
			}

			if (!Open)
			{
				AssetEditorWindowSubsystem->UnregisterAsset(Path);
			}
		}

		ImGui::EndTabBar();
	}
}
