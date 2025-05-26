#pragma once

#include "Editor/EditorWindow.h"

class LAssetEditorWindow : public LEditorWindow
{
public:
	LAssetEditorWindow(const char* iWindowName) : LEditorWindow{ iWindowName } {}

protected:
	virtual void DrawWindow() final;
};

