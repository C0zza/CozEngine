#pragma once

class LEditorWindow
{
public:
	LEditorWindow(const char* iWindowName) : WindowName{iWindowName} {}
	virtual ~LEditorWindow() {}

	virtual void Draw() = 0;

	const char* GetWindowName() const { return WindowName; }

private:
	const char* WindowName;
};

