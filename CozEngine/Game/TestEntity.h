#pragma once

#include "ECS/Entity.h"

class CTestEntity : public LEntity
{
public:
	CTestEntity();

#if defined(COZ_EDITOR)
	virtual const char* GetTypeName() const override { return "CTestEntity"; }
#endif
};
REGISTER_ENTITY(CTestEntity)

