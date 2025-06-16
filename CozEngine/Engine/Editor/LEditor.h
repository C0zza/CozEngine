#pragma once

#include <memory>
#include <set>
#include <vector>

#include "Editor/EditorWindow.h"
#include "Rendering/FrameBuffer.h"
#include "Subsystem.h"

class LEditorWindow;
class LRenderer;
class LSettingsManager;

class LEditor : public LSubsystem
{
public:
	virtual void Initialize() override;

	void Draw();

	LFrameBuffer* GetSceneFrameBuffer() { return SceneFrameBuffer.get(); }
	LFrameBuffer* GetEntityFrameBuffer() { return EntityFrameBuffer.get(); }

	template<typename TEditorWindow>
	void RegisterWindow(const char* WindowName);

	void UnregisterWindow(const std::string& WindowName);

private:
	LRenderer* Renderer = nullptr;
	LSettingsManager* SettingsManager = nullptr;

	std::unique_ptr<LFrameBuffer> SceneFrameBuffer;
	std::unique_ptr<LFrameBuffer> EntityFrameBuffer;

	std::map<std::string, std::unique_ptr<LEditorWindow>> EditorWindows;

	std::set<std::string> WindowsToUnregister;
};

template<typename TEditorWindow>
inline void LEditor::RegisterWindow(const char* WindowName)
{
	TEditorWindow* EditorWindow = new TEditorWindow(WindowName);
	if (!EditorWindow)
	{
		return;
	}

	EditorWindows.emplace(EditorWindow->GetWindowName(), std::unique_ptr<TEditorWindow>(EditorWindow));
}
