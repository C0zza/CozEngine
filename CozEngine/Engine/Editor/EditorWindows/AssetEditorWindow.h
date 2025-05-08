#pragma once

#include "Editor/EditorWindow.h"

class LAssetEditorWindow : public LEditorWindow
{
public:
	LAssetEditorWindow(const char* iWindowName) : LEditorWindow{ iWindowName } {}

	virtual void Draw() final;
};

