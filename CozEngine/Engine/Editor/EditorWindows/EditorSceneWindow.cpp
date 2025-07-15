#include "stdafx.h"

#include "EditorSceneWindow.h"

#include <filesystem>
#include <fstream>

#include "ECS/ECS2/ArchetypeManager.h"
#include "ECS/Entity.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Editor/AssetRegistry.h"
#include "Editor/SelectedEntitySubsystem.h"
#include "Globes.h"
#include "json.hpp"
#include "Misc/CoordSpaceConversion.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/RendererInfo.h"
#include "ResourceManagement/ResourceManager.h"
#include "WorldManager.h"

// TEMP: While testing drag/ drop assets
#include "SpotLightEntity.h"
// ~TEMP

LEditorSceneWindow::LEditorSceneWindow(LFrameBuffer* iSceneFrameBuffer, LFrameBuffer* iEntityFrameBuffer, const char* iWindowName)
	: LEditorWindow(iWindowName), SceneFrameBuffer{ iSceneFrameBuffer }, EntityFrameBuffer{ iEntityFrameBuffer }, LocalMouseScreenCoords{ glm::vec2() }, WindowWidth{ 0.0f }, WindowHeight{ 0.0f }
{
	assert(SceneFrameBuffer);

	InputManager = CSystem.GetSubsystems().GetSubsystem<LInputManager>();
	assert(InputManager);

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_KEY_T, GLFW_PRESS), &LEditorSceneWindow::OnToggleFocus, ToggleFocusEventHandle);
	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS), &LEditorSceneWindow::OnRightMouseClicked, RightMouseClickedEventHandle);

	InputManager->RegisterActionEvent(this, KeyAction(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE), &LEditorSceneWindow::OnMouseClicked, MouseClickedEventHandle);

	SelectedEntitySubsystem = CSystem.GetSubsystems().AddSubsystem<LSelectedEntitySubsystem>();

	ResourceManager = CSystem.GetSubsystems().AddSubsystem<LResourceManager>();
}

void LEditorSceneWindow::DrawWindow()
{
	const ImVec2 ImLocalMouseScreenCoords = GetCurrentWindowMouseCoords();
	LocalMouseScreenCoords = glm::vec2(ImLocalMouseScreenCoords.x, ImLocalMouseScreenCoords.y);

	OnFocusUpdate(ImGui::IsWindowFocused());

	const ImVec2 ContentRegionAvail = ImGui::GetContentRegionAvail();

	if (SceneFrameBuffer->GetWidth() != ContentRegionAvail.x || SceneFrameBuffer->GetHeight() != ContentRegionAvail.y)
	{
		const int X = (int)ContentRegionAvail.x;
		const int Y = (int)ContentRegionAvail.y;

		glViewport(0, 0, X, Y);
		SceneFrameBuffer->RescaleBuffer(X, Y);
		EntityFrameBuffer->RescaleBuffer(X, Y);

		LRendererInfo* RendererInfo = CSystem.GetSubsystems().GetSubsystem<LRendererInfo>();
		if (RendererInfo)
		{
			RendererInfo->UpdateProjectionMatrix(X, Y);
		}

		const ImVec2 WindowSize = ImGui::GetWindowSize();

		WindowWidth = WindowSize.x;
		// Removes outer frame at top
		WindowHeight = WindowSize.y - (ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2);
	}

	ImGui::Image(
		(ImTextureID)SceneFrameBuffer->GetTexture(),
		ContentRegionAvail,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* Payload = ImGui::GetDragDropPayload();

		if (ImGui::AcceptDragDropPayload("ASSET_DRAG"))
		{
			HeldEntity = 0;
		}
		else if (Payload && Payload->IsDataType("ASSET_DRAG")) 
		{
			// TODO: Separate into method
			if (!HeldEntity)
			{
				const FContentNode* ContentNode = reinterpret_cast<FContentNode*>(Payload->Data);

				assert(ContentNode);
				assert(!ContentNode->IsDirectory());

				// TODO: Figuring out what asset type this is shouldn't be handled here
				const std::filesystem::path& Path = ContentNode->GetPath();

				if (ResourceManager->IsAssetPathType(Path, LArchetypeConfig::StaticClass()))
				{
					const glm::vec3 ScreenToWorldRayVec = LCoordSpaceConversion::ScreenToWorldPos(LocalMouseScreenCoords, WindowWidth, WindowHeight, 10.f);

					LResourceHandle<LResource> ArchetypeConfigHandle;
					ResourceManager->GetResource(Path.string(), ArchetypeConfigHandle, LArchetypeConfig::StaticClass());

					LArchetypeConfig* ArchetypeConfig = dynamic_cast<LArchetypeConfig*>(ArchetypeConfigHandle.Get());

					if (ArchetypeConfig)
					{
						LArchetypeManager* ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
						HeldEntity = ArchetypeManager->AddEntity(*ArchetypeConfig);

						SelectedEntitySubsystem->SetSelectedEntityID(HeldEntity);

						CTransformComponent* TransformComponent = ArchetypeManager->GetEntityComponent<CTransformComponent>(HeldEntity);

						if (TransformComponent)
						{
							TransformComponent->SetPosition(ScreenToWorldRayVec);
						}
					}
				}
			}
			else
			{
				const glm::vec3 ScreenToWorldRayVec = LCoordSpaceConversion::ScreenToWorldPos(LocalMouseScreenCoords, WindowWidth, WindowHeight, 10.f);

				LArchetypeManager* ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
				CTransformComponent* TransformComponent = ArchetypeManager->GetEntityComponent<CTransformComponent>(HeldEntity);
				if (TransformComponent)
				{
					TransformComponent->SetPosition(ScreenToWorldRayVec);
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
	else if (HeldEntity > 0)
	{
		SelectedEntitySubsystem->SetSelectedEntityID(0);

		LArchetypeManager* ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
		ArchetypeManager->RemoveEntity(HeldEntity);

		HeldEntity = 0;
	}
}

void LEditorSceneWindow::PushWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
}

void LEditorSceneWindow::PopWindowStyle()
{
	ImGui::PopStyleVar(2);
}

void LEditorSceneWindow::OnToggleFocus()
{
#if defined(COZ_EDITOR)
	if (InputManager->bInputEnabled)
	{
		OnFocusUpdate(false);
	}
	else
	{
		OnFocusUpdate(true);
	}
#endif
}

void LEditorSceneWindow::OnFocusUpdate(const bool bIsFocused)
{
#if defined(COZ_EDITOR)
	if (bIsFocused != InputManager->bInputEnabled)
	{
		const char* WindowName = bIsFocused ? GetWindowName() : NULL;
		ImGui::SetWindowFocus(WindowName);
		InputManager->ResetMousePositionData();
		InputManager->bInputEnabled = bIsFocused;
	}
#endif
}

void LEditorSceneWindow::OnRightMouseClicked()
{
	if (IsWindowHovered())
	{
		OnFocusUpdate(true);
	}
}

void LEditorSceneWindow::OnMouseClicked()
{
	if (!IsWindowHovered() || !EntityFrameBuffer)
	{
		return;
	}

	const glm::vec2 TexCoords = LCoordSpaceConversion::ScreenToUV(LocalMouseScreenCoords, WindowWidth, WindowHeight);

	EntityFrameBuffer->Bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	float Pixel[4];
	// TODO: Could do with a rounding method
	glReadPixels((int)TexCoords.x, (int)TexCoords.y, 1, 1, GL_RGBA, GL_FLOAT, &Pixel);

	EntityFrameBuffer->Unbind();

	LEntityID EntityID = static_cast<LEntityID>(Pixel[0] * 255) << 24 |
						static_cast<LEntityID>(Pixel[1] * 255) << 16 |
						static_cast<LEntityID>(Pixel[2] * 255) << 8 |
						static_cast<LEntityID>(Pixel[3] * 255);

	SelectedEntitySubsystem->SetSelectedEntityID(EntityID);
}

LClass* LEditorSceneWindow::GetClassFromPath(const std::filesystem::path& Path)
{
	nlohmann::json AssetJson;

	std::ifstream AssetFile(Path);
	if (!AssetFile.is_open())
	{
		Log(LLogLevel::ERROR, "LEditorSceneWindow::SpawnEntityFromPath - Failed to open file: " + Path.string());
		return nullptr;
	}

	AssetFile >> AssetJson;

	if (!AssetJson.contains("Type"))
	{
		Log(LLogLevel::ERROR, "LEditorSceneWindow::SpawnEntityFromPath - Asset: " + Path.string() + " is missing \"Type\" entry");
		return nullptr;
	}

	LClass* AssetClass = LClassRegister::Get(AssetJson["Type"]);
	if (!AssetClass)
	{
		Log(LLogLevel::ERROR, "LEditorSceneWindow::SpawnEntityFromPath - Failed to find LClass from Type: " + AssetJson["Type"]);
		return nullptr;
	}

	assert(AssetClass->IsChildOf<LEntity>());

	return AssetClass;
}
