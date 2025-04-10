#pragma once

#include "ECS/ECSDefinitions.h"
#include "Editor/EditorWindow.h"
#include "Misc/Delegate.h"

class LInspectorWindow : public LEditorWindow
{
public:
	LInspectorWindow(const char* iWindowName) : LEditorWindow(iWindowName) {}

	virtual void Draw();
};

