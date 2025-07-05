#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"

// Temporarily inherit LTransform to avoid rewriting it all here
REFL_CLASS()
struct CTransformComponent : public LECSComponent
{
	REFL_GENERATED_BODY(CTransformComponent)
public:
	CTransformComponent();

	REFL_PROP(Visible)
	glm::vec3 Position;
	REFL_PROP(Visible)
	glm::vec3 Rotation;
	REFL_PROP(Visible)
	glm::vec3 Scale;

public:
	const glm::vec3& GetPosition() const { return Position; }
	const glm::vec3& GetRotation() const { return Rotation; }
	const glm::vec3& GetScale() const { return Scale; }

	void SetPosition(const glm::vec3& NewPosition) { Position = NewPosition; IsDirty = true; }
	void SetRotation(const glm::vec3& NewRotation) { Rotation = NewRotation; IsDirty = true; }
	void SetScale(const glm::vec3& NewScale) { Scale = NewScale; IsDirty = true; }

	void Move(const glm::vec3& Movement);
	void MoveRelative(const glm::vec3& Movement);
	void Rotate(const glm::vec3& Rotate);
	void ScaleByFloat(const float Scalar) { Scale *= Scalar; IsDirty = true; }

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

	void UpdateTransformationMatrix();
	const glm::mat4& GetTransformationMatrix() const;
	const glm::mat4& GetUpdatedTransformationMatrix();

	bool GetIsDirty() const { return IsDirty; }

private:
	glm::mat4 TransformationMatrix;
	bool IsDirty = true;
};

class CTransformComponentSystem : public LComponentSystem<CTransformComponent>
{
public:
	virtual const char* GetComponentName() const final { return "TransformComponentSystem"; }

#if defined(COZ_EDITOR)
	virtual void DrawImGuiComponent(CTransformComponent& Component) final;
#endif

protected:
	virtual void GetSerializedComponent(const CTransformComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CTransformComponent& Component, const nlohmann::json& J) final;
};

