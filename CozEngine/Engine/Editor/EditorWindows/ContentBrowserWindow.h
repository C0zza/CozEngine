#pragma once

#include <vector>

#include "Editor/EditorWindow.h"
#include "Input/InputManager.h"
#include "Rendering/Texture.h"
#include "ResourceManagement/ResourceHandle.h"

class LContentBrowserWindow : public LEditorWindow
{
public:
	LContentBrowserWindow(const char* WindowName);

protected:
	virtual void DrawWindow() final;

private:
	void DrawFolder(const std::string& Name, const std::string Path);
	void DrawAsset(const std::filesystem::path& Path);

	std::string CurrentDirectory = "";
	std::string HoveredItemPath = "";

	LResourceHandle<LTexture> FolderIcon;
};

