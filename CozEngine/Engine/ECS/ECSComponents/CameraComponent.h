#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Reflection/Reflection.h"

class LECS;

REFL_CLASS()
struct CCameraComponent : public LECSComponent
{
	REFL_GENERATED_BODY(CCameraComponent)
public:
	glm::mat4 CachedViewMatrix;
};

class CCameraComponentSystem : public LComponentSystem<CCameraComponent>
{
public:
	const glm::mat4& GetViewMatrix();
	const glm::vec3& GetViewPos();

	void ActivateCamera(const CCameraComponent& Component) { ActiveCameraEntityID = Component.EntityID; }
	int GetActiveCameraEntityID() { return ActiveCameraEntityID; }

	virtual const char* GetComponentName() const final { return "CameraComponentSystem"; }

	// TODO: Camera component doesn't need to draw anything. We should have bool to check for this
#if defined(COZ_EDITOR)
	virtual void DrawImGuiComponent(CCameraComponent& Component) final {};
#endif

protected:
	virtual void GetSerializedComponent(const CCameraComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CCameraComponent& Component, const nlohmann::json& J) final;

private:
	int ActiveCameraEntityID = -1;
};



