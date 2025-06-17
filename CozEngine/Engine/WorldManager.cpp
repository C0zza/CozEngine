#include "stdafx.h"

#include "WorldManager.h"

#if defined(COZ_EDITOR)
#include "ECS/EntityManagement/World.h"
#include "Editor/EditorSettings.h"
#include "Globes.h"
#include "Settings/SettingsManager.h"
#endif

void LWorldManager::Initialize()
{
#if defined(COZ_EDITOR)
	LSettingsManager* SettingsManager = CSystem.GetSubsystems().GetSubsystem<LSettingsManager>();
	assert(SettingsManager);

	LEditorSettings* EditorSettings = SettingsManager->GetSettings<LEditorSettings>();
	assert(EditorSettings);

	if (EditorSettings->EditorStartupLevel.ResourcePath.empty())
	{
		ActiveWorld = std::make_unique<LWorld>();
		return;
	}

	ActiveWorld = std::make_unique<LWorld>(EditorSettings->EditorStartupLevel.ResourcePath.string());
#endif
}
