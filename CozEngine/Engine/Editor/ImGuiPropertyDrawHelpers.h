#pragma once

#include "ECS/ECSComponents/TransformComponent.h"
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
				Buffer.resize(AssetPath.length());
				Buffer = std::vector<char>(AssetPath.begin(), AssetPath.end());
			}
			else
			{
				Buffer.resize(128);
				AssetPath = "";
			}

			Buffer.push_back('\0');

			if (ImGui::InputText(GetHiddenLabel(Label).c_str(), Buffer.data(), Buffer.size(), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				// TODO: Incorrect need to load through resource manager
				// ResourceHandle->SetAssetPath(Buffer.data());
			}
		}

	private:
		static std::string GetHiddenLabel(const char* Label);
	};
};


