#pragma once

#include "ECS/Entity.h"

class LModel;
class LMaterial;

class CSpotLightEntity : public LEntity 
{
public:
	CSpotLightEntity(std::shared_ptr<LModel>& Model, std::shared_ptr<LMaterial>& Material);
};

