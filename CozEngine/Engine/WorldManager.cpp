#include "stdafx.h"

#include "WorldManager.h"

#if defined(COZ_EDITOR)
#include "ECS/EntityManagement/World.h"
#include "Editor/EditorSettings.h"
#include "Globes.h"
#include "Settings/SettingsManager.h"

// TODO
#include "PlayerEntity.h"
// ~TODO
#endif

void LWorldManager::Initialize()
{
#if defined(COZ_EDITOR)
	LSettingsManager* SettingsManager = CSystem.GetSubsystems().GetSubsystem<LSettingsManager>(true);
	assert(SettingsManager);

	LEditorSettings* EditorSettings = SettingsManager->GetSettings<LEditorSettings>();
	assert(EditorSettings);

	if (EditorSettings->EditorStartupLevel.ResourcePath.empty())
	{
		ActiveWorld = std::make_unique<LWorld>();
		// TODO - Add default engine entity/ archetype to be utilised in editor.
		ActiveWorld->AddEntityByClass(CPlayerEntity::StaticClass());
		// ~ TODO
		return;
	}

	ActiveWorld = std::make_unique<LWorld>(EditorSettings->EditorStartupLevel.ResourcePath.string());
#endif
}
