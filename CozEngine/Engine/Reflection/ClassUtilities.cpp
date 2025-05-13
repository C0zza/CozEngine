#include "stdafx.h"

#include "ClassUtilities.h"

#include <utility>

#include "Class.h"
#include "Misc/Logging.h"

void LClassUtilities::RegisterStaticClassProperties(LClass* Class, std::vector<LProperty>& Properties)
{
	if (!Class)
	{
		Log(LLogLevel::ERROR, "LClassUtilities::RegisterStaticClassProperties - Invalid Class.");
		return;
	}

	Class->Properties = std::move(Properties);
}
