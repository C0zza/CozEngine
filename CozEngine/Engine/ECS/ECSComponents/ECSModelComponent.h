#pragma once

#include <memory>

#include "ECS/ComponentSystem.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"

// TODO: rename to something decent once we can get rid of the existing CModelComponent
struct CECSModelComponent
{
	std::shared_ptr<LModel> Model;
	std::shared_ptr<LMaterial> Material;
	LTransform* EntityTransform;

	CECSModelComponent(std::shared_ptr<LModel>& i_Model, std::shared_ptr<LMaterial>& i_Material, LTransform* i_EntityTransform)
		: EntityTransform{ i_EntityTransform }
	{
		Model = i_Model;
		Material = i_Material;
	}
};

class CECSModelComponentSystem : public LComponentSystem<CECSModelComponent>
{
public:
	CECSModelComponentSystem() { IsTickable = true; }

	virtual void RunComponent(CECSModelComponent& Component) override;
};

