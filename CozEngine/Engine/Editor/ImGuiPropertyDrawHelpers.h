#pragma once

#include <map>
#include <vector>

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
		static bool DrawProperty(const char* Label, int& Value);
		static bool DrawProperty(const char* Label, float& Value);
		static bool DrawProperty(const char* Label, bool& Value);
		static bool DrawProperty(const char* Label, glm::vec3& Vec3);
		static bool DrawProperty(const char* Label, std::string& Value);

		// Override for this since it's transform matrix is updated internally via setters
		static bool DrawProperty(const char* Label, CTransformComponent& TransformComponent);

		template<typename T>
		static bool DrawProperty(const char* Label, LResourceHandle<T>& ResourceHandle)
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
					return false;
				}

				LResourceHandle<T> NewResource;
				ResourceManager->GetResource<T>(Buffer.data(), NewResource);

				if (NewResource.Get())
				{
					ResourceHandle = NewResource;
				}

				return true;
			}
			return false;
		}

		template<typename TKey, typename TValue>
		static bool DrawProperty(const char* Label, std::map<TKey, TValue>& Map)
		{
			ImGui::NewLine();

			std::vector<TKey> KeysToRemove;
			std::map<TKey, TValue> PairsToAdd;

			bool bUpdated = false;

			for (std::pair<const TKey, TValue>& Pair : Map)
			{
				TKey KeyCopy = Pair.first;
				if (PrivateHelpers::DrawProperty(Label, KeyCopy))
				{
					bUpdated = true;
					KeysToRemove.emplace_back(Pair.first);
					PairsToAdd.emplace(KeyCopy, Pair.second);
				}

				ImGui::SameLine();

				bUpdated |= PrivateHelpers::DrawProperty(Label, Pair.second);
			}

			for (TKey& Key : KeysToRemove)
			{
				Map.erase(Key);
			}

			for (std::pair<const TKey, TValue>& Pair : PairsToAdd)
			{
				Map.emplace(Pair.first, Pair.second);
			}

			return bUpdated;
		}

		template<typename T>
		static bool DrawProperty(const char* Label, std::vector<T>& Vector)
		{
			ImGui::NewLine();

			bool bUpdated = false;
			for (int i = 0; i < Vector.size(); ++i)
			{
				bUpdated |= PrivateHelpers::DrawProperty(Label + i, Vector[i]);
			}

			return bUpdated;
		}

		template<typename T>
		static bool DrawProperty(const char* Label, T& Enum)
		{
			static_assert(std::is_enum_v<T>);
			return false;

			// TODO
		}

		static void RightAlignNextElement();

	private:
		static std::string GetHiddenLabel(const char* Label);
	};
};


