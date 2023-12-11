#pragma once

#include "ECS/Entity.h"

class LModel;
class LMaterial;

class CTestEntity : LEntity
{
public:
	CTestEntity(LECS* i_ECS, std::shared_ptr<LModel>& Model, std::shared_ptr<LMaterial>& Material);
};

