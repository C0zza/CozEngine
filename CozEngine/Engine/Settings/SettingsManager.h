#pragma once

#include "Subsystem.h"

#include <functional>
#include <map>
#include <memory>

#include "Settings/Settings.h"

class LClass;

class LSettingsManager : public LSubsystem
{
public:
	virtual void Initialize() override;

	void ForEachSettingsClass(std::function<void(LClass*, LSettings*)> Func);

	void SaveSettings(LClass* SettingsClass);
	void LoadSettings(LClass* SettingsClass);

private:
	std::string GetSavedSettingsPath(LClass* SettingsClass);

	std::map<LClass*, std::unique_ptr<LSettings>> SettingsClasses;
};

