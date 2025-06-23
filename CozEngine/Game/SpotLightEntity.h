#pragma once

#include "ECS/Entity.h"
#include "Reflection/Reflection.h"

REFL_CLASS()
class CSpotLightEntity : public LEntity 
{
	REFL_GENERATED_BODY(CSpotLightEntity)
public:
	CSpotLightEntity();

#if defined(COZ_EDITOR)
	virtual const char* GetTypeName() const override { return "CSpotLightEntity"; }
#endif
};
REGISTER_ENTITY(CSpotLightEntity)
