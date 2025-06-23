#pragma once

#include "Editor/EditorWindow.h"

class LECS;
class LWorldManager;
class LSelectedEntitySubsystem;

class LSceneOutlinerWindow : public LEditorWindow
{
public:
	LSceneOutlinerWindow(const char* WindowName);

protected:
	virtual void DrawWindow() override;

private:
	LECS* ECS = nullptr;
	LWorldManager* WorldManager = nullptr;
	LSelectedEntitySubsystem* SelectedEntitySubsystem = nullptr;
};

