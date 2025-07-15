#pragma once

#include "ECS/ECS2/EntityProcessor.h"

class LDirectionalLightSubsystem;

REFL_CLASS()
class LSpotLightProcessor : public LEntityProcessor
{
	REFL_GENERATED_BODY(LSpotLightProcessor)
public:
	virtual void Initialize() override;

	virtual void Execute(FEntityQueryResult& EntityQueryResult) override;

private:
	LDirectionalLightSubsystem* DirectionalLightSubsystem = nullptr;
};

