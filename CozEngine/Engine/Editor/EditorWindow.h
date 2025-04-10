#pragma once

class LEditorWindow
{
public:
	LEditorWindow(const char* iWindowName) : WindowName{iWindowName} {}
	virtual ~LEditorWindow() {}

	virtual void Draw() = 0;

	virtual void PushWindowStyle() {}
	virtual void PopWindowStyle() {}

	const char* GetWindowName() const { return WindowName; }

private:
	const char* WindowName;
};

