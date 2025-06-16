#pragma once

#include "Editor/EditorWindow.h"

class LClass;
class LSettings;
class LSettingsManager;

class LSettingsWindow : public LEditorWindow
{
public:
	LSettingsWindow(const char* WindowName);

	virtual bool ShouldDisplayCloseButton() const override final { return true; }

protected:
	virtual void DrawWindow() final;

private:
	LClass* SelectedSettingsClass = nullptr;
	LSettings* SelectedSettingsObject = nullptr;

	LSettingsManager* SettingsManager = nullptr;
};

