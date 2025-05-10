#pragma once

#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"
#include "ResourceManagement/ResourceHandle.h"
#include "ResourceManagement/ResourceManager.h"

class LImGuiPropertyDrawHelpers
{
	class PrivateHelpers;
public:
	template<typename TPropertyType>
	static void DrawProperty(const char* Label, TPropertyType& Property)
	{
		ImGui::Text(Label);
		ImGui::SameLine();

		const float yBefore = ImGui::GetCursorPosY();

		PrivateHelpers::RightAlignNextElement();

		PrivateHelpers::DrawProperty(Label, Property);

		if (yBefore == ImGui::GetCursorPosY())
		{
			ImGui::NewLine();
		}
	}

private:
	class PrivateHelpers
	{
	public:
		static void DrawProperty(const char* Label, int& Value);
		static void DrawProperty(const char* Label, float& Value);
		static void DrawProperty(const char* Label, glm::vec3& Vec3);

		// Override for this since it's transform matrix is updated internally via setters
		static void DrawProperty(const char* Label, CTransformComponent& TransformComponent);

		template<typename T>
		static void DrawProperty(const char* Label, LResourceHandle<T>& ResourceHandle)
		{
			std::string AssetPath;
			std::vector<char> Buffer;

			if (ResourceHandle.Get())
			{
				AssetPath = ResourceHandle->GetAssetPath();
				Buffer = std::vector<char>(AssetPath.begin(), AssetPath.end());
				Buffer.resize(AssetPath.length() * 2);
			}
			else
			{
				AssetPath = "";
			}

			Buffer.push_back('\0');

			if (ImGui::InputText(GetHiddenLabel(Label).c_str(), Buffer.data(), 128, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
				if (!ResourceManager)
				{
					Log(LLogLevel::ERROR, "LImGuiPropertyDrawHelpers::DrawProperty - LResourceHandle - Invalid Resource Manager.");
					return;
				}

				LResourceHandle<T> NewResource;
				ResourceManager->GetResource<T>(Buffer.data(), NewResource);

				if (NewResource.Get())
				{
					ResourceHandle = NewResource;
				}
			}
		}

		static void RightAlignNextElement();

	private:
		static std::string GetHiddenLabel(const char* Label);
	};
};


