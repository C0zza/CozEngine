#pragma once

#include <vector>

#include "Component.h"

class LModel;
class LTransform;
class LMaterial;

class CModelComponent : public CComponent
{
	friend class Renderer;
public:
	virtual void Tick() override;

	void SetModel(std::shared_ptr<LModel> i_Model) { Model = i_Model; }
	void SetMaterial(std::shared_ptr<LMaterial>& i_Material) { Material = i_Material; }

private:
	std::shared_ptr<LModel> Model;
	std::shared_ptr<LMaterial> Material;
};

