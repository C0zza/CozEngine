#include "stdafx.h"

#include "WorldManager.h"

#if defined(COZ_EDITOR)
#include "CameraManager.h"
#include "ECS/ECS2/ArchetypeManager.h"
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
		if (!EditorSettings->EditorCameraArchetypeConfig.ResourcePath.empty())
		{
			LResourceManager* ResourceManager = CSystem.GetSubsystems().GetSubsystem<LResourceManager>();

			LResourceHandle<LArchetypeConfig> EditorCameraArchetypeConfig;

			ResourceManager->GetResource<LArchetypeConfig>(EditorSettings->EditorCameraArchetypeConfig.ResourcePath, EditorCameraArchetypeConfig);

			if (EditorCameraArchetypeConfig.Get())
			{
				LArchetypeManager* ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
				
				LEntityID EditorCameraEntityID = ArchetypeManager->AddEntity(*EditorCameraArchetypeConfig.Get());

				LCameraManager* CameraManager = CSystem.GetSubsystems().GetSubsystem<LCameraManager>(true);

				CameraManager->ActivateCamera(EditorCameraEntityID);
			}
		}
		return;
	}

	ActiveWorld = std::make_unique<LWorld>(EditorSettings->EditorStartupLevel.ResourcePath.string());
#endif
}
