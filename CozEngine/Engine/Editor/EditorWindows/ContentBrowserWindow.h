#pragma once

#include <vector>

#include "Editor/ContentNodeHandle.h"
#include "Editor/EditorWindow.h"
#include "Rendering/Texture.h"
#include "ResourceManagement/ResourceHandle.h"

struct FContentNodeHandle;

class LContentBrowserWindow : public LEditorWindow
{
public:
	LContentBrowserWindow(const char* WindowName);

protected:
	virtual void DrawWindow() final;

private:
	void DrawFolder(FContentNodeHandle& NodeHandle);
	void DrawAsset(FContentNodeHandle& NodeHandle);

	FContentNodeHandle CurrentNode;
	FContentNodeHandle HoveredNode;

	LResourceHandle<LTexture> FolderIcon;
};

