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

	AssetRegistry = CSystem.GetSubsystems().GetSubsystem<LAssetRegistry>();
	assert(AssetRegistry);
}

void LContentBrowserWindow::DrawWindow()
{
	CurrentNodeHandle.Validate();

	bool bBackButtonDisabled = false;
	if (CurrentNodeHandle.GetNode().IsRoot())
	{
		bBackButtonDisabled = true;
		ImGui::BeginDisabled();
	}

	if (ImGui::Button("Back"))
	{
		// Should not be able to reach here while CurrentDirectory is empty since the button should be disabled
		assert(!CurrentNodeHandle.GetNode().IsRoot());
		CurrentNodeHandle.StepOut();
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

	if (ImGui::Button("Create Folder"))
	{
		AssetRegistry->CreateFolder(CurrentNodeHandle, "NewFolder");
	}

	ImGui::SameLine();

	if (ImGui::Button("Create Asset"))
	{

		AssetRegistry->CreateAsset(AssetPath, CurrentClass, &RenamingNodeHandle);
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

	CurrentNodeHandle.ForEachChildNode([this](FContentNodeHandle& NodeHandle)
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

	// Any file hierarchy changes must be process after iteration
	if (RenamingNodeHandle.IsValid() && !RenamedAssetName.empty())
	{
		AssetRegistry->RenameNode(RenamingNodeHandle, RenamedAssetName);
		RenamingNodeHandle.Reset();
		RenamedAssetName.clear();
	}
}

void LContentBrowserWindow::DrawFolder(FContentNodeHandle& NodeHandle)
{
	ImGui::BeginGroup();

	const FContentNode& FolderNode = NodeHandle.GetNode();

	const std::string SelectableID = "##Folder" + FolderNode.GetDisplayName();
	if (ImGui::Selectable(SelectableID.c_str(), false, CozEngine_ImGuiSelectableFlags_NoPadWithHalfSpacing | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			CurrentNodeHandle = NodeHandle;
		}
	}

	if (FolderIcon.Get())
	{
		ImVec2 SelectableMin = ImGui::GetItemRectMin();
		ImVec2 CurrentCursorScreenPos = ImGui::GetCursorScreenPos();

		ImGui::SetCursorScreenPos(SelectableMin + ImVec2(10, 10));

		ImGui::Image((ImTextureID)(intptr_t)FolderIcon->GetTextureID(), ImVec2(80, 80));

		ImGui::SetCursorScreenPos(CurrentCursorScreenPos);
	}

	DrawSharedImGui(NodeHandle, FolderNode, SelectableID);

	ImGui::EndGroup();

	// TODO: Add test for whether another item will overflow window area. If not, call ImGui::SameLine
	ImGui::SameLine();
}

void LContentBrowserWindow::DrawAsset(FContentNodeHandle& NodeHandle)
{
	ImGui::BeginGroup();

	const FContentNode& AssetNode = NodeHandle.GetNode();

	std::string SelectableID = "##Asset" + AssetNode.GetDisplayName();

	if (ImGui::Selectable(SelectableID.c_str(), false, CozEngine_ImGuiSelectableFlags_NoPadWithHalfSpacing | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			LAssetEditorWindowSubsystem* AssetEditorWindowSubsystem = CSystem.GetSubsystems().GetSubsystem<LAssetEditorWindowSubsystem>(true);
			if (AssetEditorWindowSubsystem)
			{
				AssetEditorWindowSubsystem->RegisterAsset(AssetNode.GetPath());
			}
			else
			{
				Log(LLogLevel::ERROR, "LContentBrowserWindow::DrawAsset - Invalid AssetEditorWindowSubsystem.");
			}
		}
	}

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("ASSET_DRAG", &AssetNode, sizeof(FContentNode));
		ImGui::Text(AssetNode.GetDisplayName().c_str());
		ImGui::EndDragDropSource();
	}

	DrawSharedImGui(NodeHandle, AssetNode, SelectableID);

	ImGui::EndGroup();

	// TODO: Add test for whether another item will overflow window area. If not, call ImGui::SameLine
	ImGui::SameLine();
}

void LContentBrowserWindow::DrawSharedImGui(FContentNodeHandle& NodeHandle, const FContentNode& Node, const std::string& SelectableID)
{
	bool bRenameStarted = false;

	ImGui::OpenPopupOnItemClick(SelectableID.c_str(), ImGuiPopupFlags_MouseButtonRight);
	if (ImGui::BeginPopup(SelectableID.c_str()))
	{
		if (ImGui::Selectable("Rename"))
		{
			RenamingNodeHandle = NodeHandle;
			bRenameStarted = true;
			bRenamingActive = true;
		}

		if (ImGui::Selectable("Delete"))
		{
			// AssetRegistry->DeleteNode(AssetNode.GetPath());
		}

		ImGui::EndPopup();
	}

	ImGui::PushClipRect(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(100, ImGui::GetTextLineHeight()), true);

	const std::string DisplayName = Node.GetDisplayName();
	if (bRenamingActive && RenamingNodeHandle.IsValid() && RenamingNodeHandle == NodeHandle)
	{
		// TODO: Abstract
		std::vector<char> Buffer;

		Buffer = std::vector<char>(DisplayName.begin(), DisplayName.end());
		Buffer.resize(std::max(static_cast<int>(DisplayName.length()) * 2, 100));

		Buffer.push_back('\0');

		if (bRenameStarted)
		{
			ImGui::SetKeyboardFocusHere();
		}

		// To align input text with regular text
		const ImVec2 OldPadding = ImGui::GetStyle().FramePadding;
		ImGui::GetStyle().FramePadding = ImVec2(0, 0);

		// PushClipRect is only relevant for rendering. The "physical" layout will still extend outside so we must manually enforce the width
		ImGui::SetNextItemWidth(100);

		const std::string HiddenLabel = "##" + DisplayName;
		if (ImGui::InputText(HiddenLabel.c_str(), Buffer.data(), Buffer.size(), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			RenamedAssetName = Buffer.data();
		}

		if (!bRenameStarted && !ImGui::IsItemActive())
		{
			bRenamingActive = false;
		}

		// TODO: ON_SCOPE_EXIT like macro
		ImGui::GetStyle().FramePadding = OldPadding;
	}
	else
	{
		ImGui::Text(DisplayName.c_str());
	}

	ImGui::PopClipRect();
}
