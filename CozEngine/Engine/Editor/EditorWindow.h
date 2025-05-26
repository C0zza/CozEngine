#pragma once

class LEditorWindow
{
public:
	LEditorWindow(const char* iWindowName) : WindowName{iWindowName} {}
	virtual ~LEditorWindow() {}

	void Draw();

	virtual void PushWindowStyle() {}
	virtual void PopWindowStyle() {}

	const char* GetWindowName() const { return WindowName; }

protected:
	virtual void DrawWindow() = 0;

	bool IsWindowHovered() const { return bIsWindowHovered; }

private:
	const char* WindowName;

	bool bIsWindowHovered = false;
};

