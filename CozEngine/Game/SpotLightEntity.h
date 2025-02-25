#pragma once

#include "ECS/Entity.h"

class CSpotLightEntity : public LEntity 
{
public:
	CSpotLightEntity();

#if defined(COZ_EDITOR)
	virtual const char* GetTypeName() const override { return "CSpotLightEntity"; }
#endif
};
REGISTER_ENTITY(CSpotLightEntity)
