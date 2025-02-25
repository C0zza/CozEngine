#pragma once

#include "ECS/Entity.h"

class CDirectionalLightEntity : public LEntity
{
public:
	CDirectionalLightEntity();

#if defined(COZ_EDITOR)
	virtual const char* GetTypeName() const override { return "CDirectionalLightEntity"; }
#endif
};
REGISTER_ENTITY(CDirectionalLightEntity)

