#include "stdafx.h"

#include "ContentBrowserWindow.h"

#include <filesystem>

#include "Editor/AssetEditorWindowSubsystem.h"
#include "Editor/AssetRegistry.h"
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

	LAssetRegistry* AssetRegistry = CSystem.GetSubsystems().GetSubsystem<LAssetRegistry>(true);
	assert(AssetRegistry);
}

void LContentBrowserWindow::DrawWindow()
{
	HoveredNode.Reset();

	bool bBackButtonDisabled = false;
	if (CurrentNode.GetNode().IsRoot())
	{
		bBackButtonDisabled = true;
		ImGui::BeginDisabled();
	}

	if (ImGui::Button("Back"))
	{
		// Should not be able to reach here while CurrentDirectory is empty since the button should be disabled
		assert(!CurrentNode.GetNode().IsRoot());
		CurrentNode.StepOut();
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

	CurrentNode.ForEachChildNode([this](FContentNodeHandle& NodeHandle)
		{
			if (NodeHandle.GetNode().IsDirectory())
			{
				DrawFolder(NodeHandle);
			}
			else
			{
				DrawAsset(NodeHandle);
			}
		});
}

void LContentBrowserWindow::DrawFolder(FContentNodeHandle& NodeHandle)
{
	ImGui::BeginGroup();

	if (ImGui::Selectable("##Folder", false, CozEngine_ImGuiSelectableFlags_NoPadWithHalfSpacing | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			CurrentNode = NodeHandle;
		}
	}

	if (ImGui::IsItemHovered())
	{
		HoveredNode = NodeHandle;
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
	ImGui::Text(NodeHandle.GetNode().GetDisplayName().c_str());
	ImGui::PopClipRect();

	ImGui::EndGroup();

	// TODO: Add test for whether another item will overflow window area. If not, call ImGui::SameLine
	ImGui::SameLine();
}

void LContentBrowserWindow::DrawAsset(FContentNodeHandle& NodeHandle)
{
	ImGui::BeginGroup();

	if (ImGui::Selectable("Asset", false, CozEngine_ImGuiSelectableFlags_NoPadWithHalfSpacing | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			LAssetEditorWindowSubsystem* AssetEditorWindowSubsystem = CSystem.GetSubsystems().GetSubsystem<LAssetEditorWindowSubsystem>(true);
			if (AssetEditorWindowSubsystem)
			{
				AssetEditorWindowSubsystem->RegisterAsset(NodeHandle.GetNode().GetPath());
			}
			else
			{
				Log(LLogLevel::ERROR, "LContentBrowserWindow::DrawAsset - Invalid AssetEditorWindowSubsystem.");
			}
		}
	}

	if (ImGui::IsItemHovered())
	{
		HoveredNode = NodeHandle;
	}

	ImGui::PushClipRect(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(100, ImGui::GetTextLineHeight()), true);
	ImGui::Text(NodeHandle.GetNode().GetDisplayName().c_str());
	ImGui::PopClipRect();

	ImGui::EndGroup();

	// TODO: Add test for whether another item will overflow window area. If not, call ImGui::SameLine
	ImGui::SameLine();
}
