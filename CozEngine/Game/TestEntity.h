#pragma once

#include "ECS/Entity.h"

class LModel;
class LMaterial;

class CTestEntity : public LEntity
{
public:
	CTestEntity(std::shared_ptr<LModel>& Model, std::shared_ptr<LMaterial>& Material);
};

