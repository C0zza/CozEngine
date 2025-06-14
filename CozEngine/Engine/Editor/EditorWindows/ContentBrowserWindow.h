#pragma once

#include <vector>

#include "Editor/ContentNodeHandle.h"
#include "Editor/EditorWindow.h"
#include "Rendering/Texture.h"
#include "ResourceManagement/ResourceHandle.h"

class LAssetRegistry;

class LContentBrowserWindow : public LEditorWindow
{
public:
	LContentBrowserWindow(const char* WindowName);

protected:
	virtual void DrawWindow() final;

private:
	void DrawFolder(FContentNodeHandle& NodeHandle);
	void DrawAsset(FContentNodeHandle& NodeHandle);

	void DrawSharedImGui(FContentNodeHandle& NodeHandle, const FContentNode& Node, const std::string& SelectableID);

	FContentNodeHandle CurrentNodeHandle;

	FContentNodeHandle RenamingNodeHandle;
	std::string RenamedAssetName = "";
	bool bRenamingActive = false;

	LResourceHandle<LTexture> FolderIcon;

	LAssetRegistry* AssetRegistry = nullptr;
};

