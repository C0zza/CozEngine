#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Globes.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "ResourceManagement/ResourceManager.h"

// TODO: rename to something decent once we can get rid of the existing CModelComponent
struct CModelComponent : public LECSComponent
{
	LResourceHandle<LModel> Model;
	LResourceHandle<LMaterial> Material;

	CModelComponent() = default;
	CModelComponent(const std::string& i_Model, const std::string& i_Material)
	{
		LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();
		ResourceManager->GetResource<LModel>(i_Model, Model);
		ResourceManager->GetResource<LMaterial>(i_Material, Material);
	}
};

class CModelComponentSystem : public LComponentSystem<CModelComponent>
{
public:
	virtual void RunComponent(CModelComponent& Component) override;

	virtual EComponentSystemType GetComponentSystemType() const { return EComponentSystemType::Renderer; }
	virtual const char* GetComponentName() const final { return "ModelComponentSystem"; }

protected:

	virtual void GetSerializedComponent(const CModelComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CModelComponent& Component, const nlohmann::json& J) final;
};

