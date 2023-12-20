#pragma once

#include <memory>

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"

// TODO: rename to something decent once we can get rid of the existing CModelComponent
struct CModelComponent : LECSComponent
{
	std::shared_ptr<LModel> Model;
	std::shared_ptr<LMaterial> Material;

	CModelComponent(std::shared_ptr<LModel>& i_Model, std::shared_ptr<LMaterial>& i_Material)
	{
		Model = i_Model;
		Material = i_Material;
	}
};

class CModelComponentSystem : public LComponentSystem<CModelComponent>
{
public:
	CModelComponentSystem() { IsTickable = true; }

	virtual void RunComponent(CModelComponent& Component) override;
};

