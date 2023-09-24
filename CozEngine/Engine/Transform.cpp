#include "Transform.h"

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

LTransform::LTransform()
	: Position{ glm::vec3(0.f, 0.f, 0.f) }, Rotation{ glm::vec3(0.f, 0.f, 0.f) }, Scale{ glm::vec3(1.f, 1.f, 1.f) },
	  TransformationMatrix { glm::mat4(1.f) }
{
}

void LTransform::Move(const glm::vec3& Movement)
{
	Position.x += Movement.x;
	Position.y += Movement.y;
	Position.z += Movement.z;
	IsDirty = true;
}

void LTransform::Rotate(const glm::vec3& Rotate)
{
	Rotation += Rotate;

	float Min = 0.f, Max = 360.f;
	Clamp(Position.x, Min, Max);
	Clamp(Position.y, Min, Max);
	Clamp(Position.z, Min, Max);

	IsDirty = true;
}

glm::vec3 LTransform::GetForward() const
{
	glm::vec3 Result = glm::vec3(0.f, 0.f, -1.f);
	Result = glm::rotateX(Result, Rotation.x);
	Result = glm::rotateY(Result, Rotation.y);
	Result = glm::rotateZ(Result, Rotation.z);
	return glm::normalize(Result);
}

void LTransform::UpdateTransformationMatrix()
{
	TransformationMatrix = glm::mat4(1.f);
	TransformationMatrix = glm::translate(TransformationMatrix, Position);
	TransformationMatrix = glm::scale(TransformationMatrix, Scale);
	TransformationMatrix = glm::rotate(TransformationMatrix, Rotation.x, glm::vec3(1.0, 0.f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, Rotation.y, glm::vec3(0.f, 1.0f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, Rotation.z, glm::vec3(0.f, 0.f, 1.0f));

	IsDirty = false;
}
