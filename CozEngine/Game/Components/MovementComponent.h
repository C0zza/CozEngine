#pragma once

#include <glm/vec3.hpp>
#include "json.hpp"
#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Reflection/Reflection.h"

REFL_CLASS()
struct CMovementComponent : public LECSComponent
{
	REFL_GENERATED_BODY(CMovementComponent)
public:
	REFL_PROP(Visible)
	glm::vec3 Movement = glm::vec3(0.f, 0.f, 0.f);
	REFL_PROP(Visible)
	float Speed = 0.05f;

	REFL_PROP(Visible)
	glm::vec3 Rotation = glm::vec3(0.f, 0.f, 0.f);
	REFL_PROP(Visible)
	float Sensitivity = 0.01f;
};

class CMovementSystem : public LComponentSystem<CMovementComponent>
{
public:
	virtual EComponentSystemType GetComponentSystemType() const { return EComponentSystemType::Ticker; }
	virtual const char* GetComponentName() const final { return "MovementSystem"; }

protected:
	virtual void GetSerializedComponent(const CMovementComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CMovementComponent& Component, const nlohmann::json& J) final;

#if defined(COZ_EDITOR)
	virtual void DrawImGuiComponent(CMovementComponent& Component) final {};
#endif

private:
	virtual void RunComponent(CMovementComponent& Component) override;
};

