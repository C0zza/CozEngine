#pragma once

#include <vector>

#include "Component.h"

class LMesh;
class LTransform;
struct LMaterial;

class CModelComponent : public CComponent
{
	friend class Renderer;
public:
	virtual void Tick() override;

	void SetMesh(std::shared_ptr<LMesh>& i_Mesh) { Mesh = i_Mesh; }
	void SetMaterial(std::shared_ptr<LMaterial>& i_Material) { Material = i_Material; }

private:
	std::shared_ptr<LMesh> Mesh;
	std::shared_ptr<LMaterial> Material;
};

