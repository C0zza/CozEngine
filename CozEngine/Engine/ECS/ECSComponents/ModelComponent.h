#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "ResourceManagement/ResourceManager.h"

// TODO: rename to something decent once we can get rid of the existing CModelComponent
struct CModelComponent : LECSComponent
{
	LResourceHandle<LModel> Model;
	LResourceHandle<LMaterial> Material;

	CModelComponent(const std::string& i_Model, const std::string& i_Material)
	{
		Model = LResourceManager::GetResource<LModel>(i_Model);
		Material = LResourceManager::GetResource<LMaterial>(i_Material);
	}
};

class CModelComponentSystem : public LComponentSystem<CModelComponent>
{
public:
	virtual void RunComponent(CModelComponent& Component) override;

protected:
	virtual void Init() override { IsTickable = true; }
};

