#include "stdafx.h"
#include "TransformComponent.h"

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Misc/GlmSerialization.h"

#if defined(COZ_EDITOR)
#include "Editor/ImGuiPropertyDrawHelpers.h"

void CTransformComponentSystem::DrawImGuiComponent(CTransformComponent& Component)
{
	LImGuiPropertyDrawHelpers::DrawProperty("Position", Component);
}
#endif

void CTransformComponentSystem::GetSerializedComponent(const CTransformComponent& Component, nlohmann::json& J) const
{
	J["Position"] = Component.GetPosition();
	J["Rotation"] = Component.GetRotation();
	J["Scale"] = Component.GetScale();
}

void CTransformComponentSystem::DeserializeComponent(CTransformComponent& Component, const nlohmann::json& J)
{
	Component.SetPosition(J["Position"]);
	Component.SetRotation(J["Rotation"]);
	Component.SetScale(J["Scale"]);
}

void Clamp(float& Value, const float Min, const float Max)
{
	if (Value > Max)
	{
		Value -= Max;
	}
	else if (Value < Min)
	{
		Value += Max;
	}
}

CTransformComponent::CTransformComponent()
	: Position{ glm::vec3(0.f, 0.f, 0.f) }, Rotation{ glm::vec3(0.f, 0.f, 0.f) }, Scale{ glm::vec3(1.f, 1.f, 1.f) },
	TransformationMatrix{ glm::mat4(1.f) }
{
}

void CTransformComponent::MoveRelative(const glm::vec3& Movement)
{
	glm::vec3 RotatedMovement = Movement;
	RotatedMovement = glm::rotateX(RotatedMovement, Rotation.x);
	RotatedMovement = glm::rotateY(RotatedMovement, Rotation.y);
	RotatedMovement = glm::rotateZ(RotatedMovement, Rotation.z);

	Position.x += RotatedMovement.x;
	Position.y += RotatedMovement.y;
	Position.z += RotatedMovement.z;
	IsDirty = true;
}

void CTransformComponent::Move(const glm::vec3& Movement)
{
	Position.x += Movement.x;
	Position.y += Movement.y;
	Position.z += Movement.z;
	IsDirty = true;
}

void CTransformComponent::Rotate(const glm::vec3& Rotate)
{
	Rotation += Rotate;

	float Min = 0.f, Max = 360.f;
	/*Clamp(Rotation.x, Min, Max);
	Clamp(Rotation.y, Min, Max);
	Clamp(Rotation.z, Min, Max);*/

	IsDirty = true;
}

glm::vec3 CTransformComponent::GetForward() const
{
	glm::vec3 Result = glm::vec3(0.f, 0.f, -1.f);
	Result = glm::rotateX(Result, Rotation.x);
	Result = glm::rotateY(Result, Rotation.y);
	Result = glm::rotateZ(Result, Rotation.z);
	return glm::normalize(Result);
}

glm::vec3 CTransformComponent::GetRight() const
{
	glm::vec3 Result = glm::vec3(1.f, 0.f, 0.f);
	Result = glm::rotateX(Result, Rotation.x);
	Result = glm::rotateY(Result, Rotation.y);
	Result = glm::rotateZ(Result, Rotation.z);
	return glm::normalize(Result);
}

glm::vec3 CTransformComponent::GetUp() const
{
	glm::vec3 Result = glm::vec3(0.f, 1.f, 0.f);
	Result = glm::rotateX(Result, Rotation.x);
	Result = glm::rotateY(Result, Rotation.y);
	Result = glm::rotateZ(Result, Rotation.z);
	return glm::normalize(Result);
}

void CTransformComponent::UpdateTransformationMatrix()
{
	TransformationMatrix = glm::mat4(1.f);
	TransformationMatrix = glm::translate(TransformationMatrix, Position);
	TransformationMatrix = glm::rotate(TransformationMatrix, Rotation.x, glm::vec3(1.0, 0.f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, Rotation.y, glm::vec3(0.f, 1.0f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, Rotation.z, glm::vec3(0.f, 0.f, 1.0f));
	TransformationMatrix = glm::scale(TransformationMatrix, Scale);

	IsDirty = false;
}

const glm::mat4& CTransformComponent::GetTransformationMatrix() const
{
	return TransformationMatrix;
}

const glm::mat4& CTransformComponent::GetUpdatedTransformationMatrix()
{
	if (IsDirty)
	{
		UpdateTransformationMatrix();
	}

	return GetTransformationMatrix();
}
