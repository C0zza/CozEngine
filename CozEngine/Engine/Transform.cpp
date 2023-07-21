#include "Transform.h"

#include <glm/common.hpp>

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

Transform::Transform()
	: Position{ glm::vec3(0.f, 0.f, 0.f) }, Rotation{ glm::vec3(0.f, 0.f, 0.f) }, Scale{ glm::vec3(1.f, 1.f, 1.f) }
{
}

void Transform::Move(const glm::vec3& Movement)
{
	Position.x += Movement.x;
	Position.y += Movement.y;
	Position.z += Movement.z;
}

void Transform::Rotate(const glm::vec3& Rotate)
{
	Rotation += Rotate;

	float Min = 0.f, Max = 360.f;
	Clamp(Position.x, Min, Max);
	Clamp(Position.y, Min, Max);
	Clamp(Position.z, Min, Max);
}
