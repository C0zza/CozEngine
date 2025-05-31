#include "stdafx.h"

#include "ContentBrowserWindow.h"

#include <filesystem>

#include "Editor/AssetEditorWindowSubsystem.h"
#include "Globes.h"
#include "Reflection/Class.h"
#include "ResourceManagement/Resource.h"

namespace
{
	std::string FolderIconTexturePath = "Engine/Content/Textures/FolderIcon.casset";
}

LContentBrowserWindow::LContentBrowserWindow(const char* WindowName)
	: LEditorWindow(WindowName)
{
	LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
	if (!ResourceManager)
	{
		Log(LLogLevel::ERROR, "LContentBrowserWindow::LContentBrowserWindow - Invalid ResourceManager.");
		return;
	}

	ResourceManager->GetResource<LTexture>(FolderIconTexturePath, FolderIcon);

	if (!FolderIcon.Get())
	{
		Log(LLogLevel::ERROR, "LContentBrowserWindow::LContentBrowserWindow - Failed to load FolderIcon with path " + FolderIconTexturePath);
		return;
	}
}

void LContentBrowserWindow::DrawWindow()
{
	constexpr const char* EngineContent = "Engine\\Content";
	constexpr const char* ProjectContent = "Game\\Content";

	HoveredItemPath.clear();

	bool bBackButtonDisabled = false;
	if (CurrentDirectory.empty())
	{
		bBackButtonDisabled = true;
		ImGui::BeginDisabled();
	}

	if (ImGui::Button("Back"))
	{
		// Should not be able to reach here while CurrentDirectory is empty since the button should be disabled
		assert(!CurrentDirectory.empty());
		if (CurrentDirectory == EngineContent || CurrentDirectory == ProjectContent)
		{
			CurrentDirectory.clear();
		}
		else
		{
			std::filesystem::path Path = CurrentDirectory;
			CurrentDirectory = Path.parent_path().string();
		}
	}

	if (bBackButtonDisabled)
	{
		ImGui::EndDisabled();
	}

	ImGui::SameLine();

	static const LClass* ResourceClass = LResource::StaticClass();
	static const std::vector<LClass*> ResourceClasses = ResourceClass->GetChildClasses();
	assert(!ResourceClasses.empty());

	static LClass* CurrentClass = ResourceClasses[0];

	if (ImGui::Button("Create Asset"))
	{

	}

	ImGui::SameLine();

	if (ImGui::BeginCombo("Type", CurrentClass->GetTypeName().data(), ImGuiComboFlags_WidthFitPreview))
	{
		for (LClass* Class : ResourceClasses)
		{
			assert(Class);

			bool bIsSelected = CurrentClass == Class;

			if (ImGui::Selectable(Class->GetTypeName().data(), &bIsSelected))
			{
				CurrentClass = Class;
			}

			if (bIsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (CurrentDirectory.empty())
	{
		assert(std::filesystem::exists(EngineContent) && std::filesystem::is_directory(EngineContent));
		assert(std::filesystem::exists(ProjectContent) && std::filesystem::is_directory(ProjectContent));

		DrawFolder("Engine", EngineContent);
		DrawFolder("Project", ProjectContent);
	}
	else
	{
		assert(std::filesystem::exists(CurrentDirectory) && std::filesystem::is_directory(CurrentDirectory));

		for (const auto& Entry : std::filesystem::directory_iterator(CurrentDirectory)) 
		{
			if (Entry.is_directory())
			{
				DrawFolder(Entry.path().filename().string(), Entry.path().string());
			}
			else if (Entry.path().extension() == ".casset")
			{
				DrawAsset(Entry.path());
			}
			else
			{
				Log(LLogLevel::WARNING, "FFolder::Populate - Unexpected filetype found in content folder: " + Entry.path().string());
			}
		}
	}
}

void LContentBrowserWindow::DrawFolder(const std::string& Name, const std::string Path)
{
	ImGui::BeginGroup();

	if (ImGui::Selectable("##Folder", false, CozEngine_ImGuiSelectableFlags_NoPadWithHalfSpacing | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			CurrentDirectory = Path;
		}
	}

	if (ImGui::IsItemHovered())
	{
		HoveredItemPath = Path;
	}

	if (FolderIcon.Get())
	{
		ImVec2 SelectableMin = ImGui::GetItemRectMin();
		ImVec2 CurrentCursorScreenPos = ImGui::GetCursorScreenPos();

		ImGui::SetCursorScreenPos(SelectableMin + ImVec2(10, 10));

		ImGui::Image((ImTextureID)(intptr_t)FolderIcon->GetTextureID(), ImVec2(80, 80));

		ImGui::SetCursorScreenPos(CurrentCursorScreenPos);
	}

	ImGui::PushClipRect(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(100, ImGui::GetTextLineHeight()), true);
	ImGui::Text(Name.c_str());
	ImGui::PopClipRect();

	ImGui::EndGroup();

	// TODO: Add test for whether another item will overflow window area. If not, call ImGui::SameLine
	ImGui::SameLine();
}

void LContentBrowserWindow::DrawAsset(const std::filesystem::path& Path)
{
	ImGui::BeginGroup();

	if (ImGui::Selectable("Asset", false, CozEngine_ImGuiSelectableFlags_NoPadWithHalfSpacing | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			LAssetEditorWindowSubsystem* AssetEditorWindowSubsystem = CSystem.GetSubsystems().GetSubsystem<LAssetEditorWindowSubsystem>(true);
			if (AssetEditorWindowSubsystem)
			{
				AssetEditorWindowSubsystem->RegisterAsset(Path);
			}
			else
			{
				Log(LLogLevel::ERROR, "LContentBrowserWindow::DrawAsset - Invalid AssetEditorWindowSubsystem.");
			}
		}
	}

	ImGui::PushClipRect(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(100, ImGui::GetTextLineHeight()), true);
	ImGui::Text(Path.stem().string().c_str());
	ImGui::PopClipRect();

	ImGui::EndGroup();

	// TODO: Add test for whether another item will overflow window area. If not, call ImGui::SameLine
	ImGui::SameLine();
}
