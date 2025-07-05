#pragma once

#include "ECS/ECS2/ArchetypeConfig.h"
#include "ECS/EntityManagement/EntityContainerAsset.h"
#include "Reflection/Reflection.h"
#include "ResourceManagement/SoftResourceHandle.h"
#include "Settings/Settings.h"

class LArchetypeConfig;
class LEntityContainerAsset;

REFL_CLASS()
class LEditorSettings : public LSettings
{
	REFL_GENERATED_BODY(LEditorSettings)
public:
	REFL_PROP(Visible)
	LSoftResourceHandle<LEntityContainerAsset> EditorStartupLevel;

	REFL_PROP(Visible)
	LSoftResourceHandle<LArchetypeConfig> EditorCameraArchetypeConfig;
};

