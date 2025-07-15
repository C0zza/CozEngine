#pragma once

#include "ECSComponent.h"
#include "Reflection/Reflection.h"

// TODO: Should probably have a more dedicated tag system, but for now empty components will do the job.

REFL_CLASS()
class LActiveTagComponent : public LECSComponent
{
	REFL_GENERATED_BODY(LActiveTagComponent)
};

