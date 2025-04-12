#include "ECS.h"

#include "Globes.h"

LECS::LECS()
{
	TypeIdGenerator = CSystem.GetSubsystems().GetSubsystem<LTypeIdGenerator>();
	assert(TypeIdGenerator);
}

void LECS::UpdateComponentSystemTypes(const EComponentSystemType ComponentSystemType)
{
	if (!SpecialzedComponentSystems.contains(ComponentSystemType) || SpecialzedComponentSystems.at(ComponentSystemType).empty())
	{
		return;
	}

	for (LComponentSystemBase* ComponentSystem : SpecialzedComponentSystems.at(ComponentSystemType))
	{
		ComponentSystem->InternalRun();
	}
}

void LECS::GetEntityComponentData(const LEntityID EntityID, nlohmann::json& ComponentData) const
{
	for (const std::pair<const LComponentTypeID, std::unique_ptr<LComponentSystemBase>>& Pair : ComponentSystems)
	{
		const std::unique_ptr<LComponentSystemBase>& ComponentSystem = Pair.second;
		if (ComponentSystem->ContainsComponent(EntityID))
		{
			nlohmann::json& Json = ComponentData[ComponentSystem->GetComponentName()];
			ComponentSystem->GetSerializedComponent(EntityID, Json);
		}
	}
}

void LECS::InitEntityComponentData(const LEntityID EntityID, const std::string& ComponentSystemName, const nlohmann::json& ComponentData)
{
	if (!ComponentSystemsByName.contains(ComponentSystemName))
	{
		Log(LLogLevel::ERROR, "LECS::InitEntityComponentData - Could not find " + ComponentSystemName + " in ComponentSystemsByName.");
		return;
	}

	LComponentSystemBase* ComponentSystem = ComponentSystemsByName[ComponentSystemName.c_str()];
	if (!ComponentSystem)
	{
		Log(LLogLevel::ERROR, "LECS::InitEntityComponentData - ComponentSystem is invalid.");
		return;
	}

	ComponentSystem->DeserializeEntityComponentID(EntityID, ComponentData);
}

LComponentSystemBase* LECS::GetComponentSystemByName(const std::string& Name)
{
	if (!ComponentSystemsByName.contains(Name))
	{
		return nullptr;
	}

	return ComponentSystemsByName.at(Name);
}
