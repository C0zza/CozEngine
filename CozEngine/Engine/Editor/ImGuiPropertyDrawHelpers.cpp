#include "stdafx.h"
#include "ImGuiPropertyDrawHelpers.h"

void LImGuiPropertyDrawHelpers::PrivateHelpers::DrawProperty(const char* Label, int& Value)
{
	int Buffer = Value;
	if (ImGui::InputInt(GetHiddenLabel(Label).c_str(), &Buffer, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		Value = Buffer;
	}
}

void LImGuiPropertyDrawHelpers::PrivateHelpers::DrawProperty(const char* Label, float& Value)
{
	float Buffer = Value;
	if(ImGui::InputFloat(GetHiddenLabel(Label).c_str(), &Buffer, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		Value = Buffer;
	}
}

void LImGuiPropertyDrawHelpers::PrivateHelpers::DrawProperty(const char* Label, glm::vec3& Vec3)
{
	float Buffer[3]{ Vec3.x, Vec3.y, Vec3.z };
	if (ImGui::InputFloat3(GetHiddenLabel(Label).c_str(), Buffer, "%.3f"))
	{
		Vec3.x = Buffer[0];
		Vec3.y = Buffer[1];
		Vec3.z = Buffer[2];
	}
}

void LImGuiPropertyDrawHelpers::PrivateHelpers::DrawProperty(const char* Label, CTransformComponent& TransformComponent)
{
	// TODO: This doesn't quite match the usual layout. Sort.

	const glm::vec3& Pos = TransformComponent.GetPosition();
	float Buffer[3]{ Pos.x, Pos.y, Pos.z };

	if (ImGui::InputFloat3("##Position", Buffer, "%.3f"))
	{
		TransformComponent.SetPosition(glm::vec3(Buffer[0], Buffer[1], Buffer[2]));
	}

	const glm::vec3& Rot = TransformComponent.GetRotation();
	Buffer[0] = Rot.x;
	Buffer[1] = Rot.y;
	Buffer[2] = Rot.z;

	ImGui::Text("Rotation");
	ImGui::SameLine();

	RightAlignNextElement();

	if (ImGui::InputFloat3("##Rotation", Buffer, "%.3f"))
	{
		TransformComponent.SetRotation(glm::vec3(Buffer[0], Buffer[1], Buffer[2]));
	}

	const glm::vec3& Sca = TransformComponent.GetScale();
	Buffer[0] = Sca.x;
	Buffer[1] = Sca.y;
	Buffer[2] = Sca.z;

	ImGui::Text("Scale");
	ImGui::SameLine();

	RightAlignNextElement();

	if (ImGui::InputFloat3("##Scale", Buffer, "%.3f"))
	{
		TransformComponent.SetScale(glm::vec3(Buffer[0], Buffer[1], Buffer[2]));
	}
}

void LImGuiPropertyDrawHelpers::PrivateHelpers::RightAlignNextElement()
{
	const float XPos = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth();
	if (XPos > ImGui::GetCursorPosX())
	{
		ImGui::SetCursorPosX(XPos);
	}
}

std::string LImGuiPropertyDrawHelpers::PrivateHelpers::GetHiddenLabel(const char* Label)
{
	return std::string("##") + Label;
}
