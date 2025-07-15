#pragma once

#include "ECS/ECS2/EntityProcessor.h"

class LDirectionalLightSubsystem;

REFL_CLASS()
class LPointLightProcessor : public LEntityProcessor
{
	REFL_GENERATED_BODY(LPointLightProcessor)
public:
	virtual void Initialize() override;

	virtual void Execute(FEntityQueryResult& EntityQueryResult) override;

private:
	LDirectionalLightSubsystem* DirectionalLightSubsystem = nullptr;
};

