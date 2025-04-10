#pragma once

#include "ECS/ECSDefinitions.h"
#include "Editor/EditorWindow.h"
#include "Misc/Delegate.h"

class LInspectorWindow : public LEditorWindow
{
public:
	LInspectorWindow(const char* iWindowName);

	virtual void Draw();

private:
	void OnEntitySelected(const LEntityID EntityID);

	LDelegateEventHandle OnEntitySelectedHandle;
};

