#pragma once

#include "ECS/Entity.h"

class CLandscapeEntity : public LEntity
{
public:
	CLandscapeEntity();

#if defined(COZ_EDITOR)
	virtual const char* GetTypeName() const override { return "CLandscapeEntity"; }
#endif
};
REGISTER_ENTITY(CLandscapeEntity)

