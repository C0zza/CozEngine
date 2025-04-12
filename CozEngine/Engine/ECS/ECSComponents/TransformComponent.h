#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Transform.h"

// Temporarily inherit LTransform to avoid rewriting it all here
struct CTransformComponent : public LTransform, LECSComponent
{
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

