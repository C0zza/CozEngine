#include "stdafx.h"
#include "TransformComponent.h"

#include "Misc/GlmSerialization.h"

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
