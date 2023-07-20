#pragma once

#include <glm/vec3.hpp>

class Transform
{
public:
	Transform();

	const glm::vec3& GetPosition() const { return Position; }
	const glm::vec3& GetRotation() const { return Rotation; }
	const glm::vec3& GetScale() const { return Scale; }

	void SetPosition(const glm::vec3& NewPosition) { Position = NewPosition; }
	void SetRotation(const glm::vec3& NewRotation) { Rotation = NewRotation; }
	void SetScale(const glm::vec3& NewScale) { Scale = NewScale; }

	void Move(const glm::vec3& Movement) { Position += Movement; }
	void Rotate(const glm::vec3& Rotate);

private:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};



