#pragma once

#include "Editor/EditorWindow.h"

class LArchetypeManager;
class LSelectedEntitySubsystem;

class LSceneOutlinerWindow : public LEditorWindow
{
public:
	LSceneOutlinerWindow(const char* WindowName);

protected:
	virtual void DrawWindow() override;

private:
	LArchetypeManager* ArchetypeManager = nullptr;
	LSelectedEntitySubsystem* SelectedEntitySubsystem = nullptr;
};

