#include "stdafx.h"

#include "ArchetypeConfig.h"

bool LArchetypeConfig::Includes(const LArchetypeConfig& Config) const
{
    for (const TSubclassOf<LECSComponent>& ComponentClass : Config.GetComponentSet())
    {
        if (!ComponentSet.contains(ComponentClass))
        {
            return false;
        }
    }

    return true;
}
