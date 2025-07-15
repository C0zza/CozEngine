#pragma once

#include "ECS/ECS2/EntityProcessor.h"

REFL_CLASS()
class LMovementProcessor : public LEntityProcessor
{
	REFL_GENERATED_BODY(LMovementProcessor)
public:
	virtual void Initialize() override;
	virtual void Execute(FEntityQueryResult& EntityQueryResult) override;
};

