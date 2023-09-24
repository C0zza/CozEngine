#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// TODO: Convert this to a component?

class LTransform
{
public:
	LTransform();

	const glm::vec3& GetPosition() const { return Position; }
	const glm::vec3& GetRotation() const { return Rotation; }
	const glm::vec3& GetScale() const { return Scale; }

	void SetPosition(const glm::vec3& NewPosition) { Position = NewPosition; IsDirty = true; }
	void SetRotation(const glm::vec3& NewRotation) { Rotation = NewRotation; IsDirty = true; }
	void SetScale(const glm::vec3& NewScale) { Scale = NewScale; IsDirty = true; }

	void Move(const glm::vec3& Movement);
	void Rotate(const glm::vec3& Rotate);

	glm::vec3 GetForward() const;

	void UpdateTransformationMatrix();
	const glm::mat4& GetTransformationMatrix() const { return TransformationMatrix; }
	bool GetIsDirty() const { return IsDirty; }

private:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	glm::mat4 TransformationMatrix;
	bool IsDirty = true;
};



